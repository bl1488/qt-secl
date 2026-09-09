#include "net/packet.h"

#include "include/spdlog-wrapper.h"

#include <QJsonObject>
#include <QIODevice>
#include <QJsonDocument>
#include <QtEndian>
#include <QTcpSocket>

//
// PacketReader
//
net::PacketReader::PacketReader(QTcpSocket* socket, QObject* parent) : 
   QObject(parent), socket_(socket) 
{
   assert(socket);

   ResetStreamState();

   connect(socket, &QTcpSocket::readyRead, this, &PacketReader::OnRead);
}

QByteArray net::CreatePacket(std::uint16_t type, const QJsonObject&  payload_json) {
   QByteArray payload = QJsonDocument(payload_json).toJson(QJsonDocument::Compact);

   QByteArray packet;
   packet.reserve(sizeof(Packet::Header) + payload.size());

   Packet::Header header;
   header.type   = qToBigEndian(std::uint16_t(type));
   header.length = qToBigEndian(std::uint16_t(payload.size()));

   packet.append(reinterpret_cast<char*>(&header), sizeof(header));
   packet.append(payload);

   return packet;
}

void net::PacketReader::OnRead() {
   assert(socket_);

   while (socket_->bytesAvailable() > 0) {
      if (stream_state_.state == StreamState::ReadHeader) {
         if (socket_->bytesAvailable() < qint64(sizeof(Packet::Header)))
            return;

         // read binary header
         Packet::Header header;
         socket_->read(reinterpret_cast<char*>(&header), sizeof(header));   

         header.length = qFromBigEndian(header.length);
         header.type   = qFromBigEndian(header.type);

         switch (header.type) {
         case Packet::ServerAccept: 
         case Packet::Start:
         case Packet::Stop:
         case Packet::Text: 
            break;
         case Packet::Unknown: 
         default:
            GlobalLogError("invalid payload type: {}:{}",
               std::uint16_t(header.type), 
               Packet::TypeToString(header.type)
            );
            return socket_->abort();
         }

         stream_state_.expected_length = header.length;
         stream_state_.state           = StreamState::ReadPayload;
         stream_state_.last_type       = header.type;

         if (stream_state_.expected_length > MAX_PAYLOAD_SIZE) {
            GlobalLogError("invalid expected_length: {}", stream_state_.expected_length);
            return socket_->abort();
         }
      }
      if (stream_state_.state == StreamState::ReadPayload) {
         if (socket_->bytesAvailable() < stream_state_.expected_length)
            return;

         // read json payload
         QByteArray raw_payload_data = socket_->read(stream_state_.expected_length);
         QJsonDocument payload_doc   = QJsonDocument::fromJson(raw_payload_data);
         if (payload_doc.isNull() || !payload_doc.isObject()) {
            GlobalLogError("failed to parse payload json");
            return socket_->abort();
         }

         emit PacketReceived(stream_state_.last_type, payload_doc.object());

         ResetStreamState();
      }
   }
}

void net::PacketReader::ResetStreamState() noexcept { 
   stream_state_.state           = StreamState::ReadHeader;
   stream_state_.expected_length = 0;
   stream_state_.last_type       = Packet::Unknown;
}
