#ifndef PACKET_H_
#define PACKET_H_

#include <QObject>
#include <QMetaType>

class QTcpSocket;
class QJsonObject;

namespace net {

#pragma pack(push, 1)

//
// Packet
//
struct Packet {
   enum Type : std::uint16_t {
      // first server message
      ServerAccept,
      // common type
      Text,
      // start/stop sender on client
      Start,
      Stop,
      // state after reset
      Unknown = 255
   };
   constexpr static const char* TypeToString(std::uint16_t type) noexcept {
      switch (type) {
      case Type::ServerAccept: return "ServerAccept";
      case Type::Text:         return "Text";
      case Type::Start:        return "Start";
      case Type::Stop:         return "Stop";
      case Type::Unknown:      return "Unknown";
      }
      return "unknown type";
   }

   struct Header {
      std::uint16_t type;
      std::uint16_t length;
   };

public:
   Header     header;
   QByteArray payload;
};

#pragma pack(pop)

//
// PacketReader
//
// wrapper around <readyRead> signal (QTcpSocket).
// server and client simply connect to the <PacketReceived> signal
class PacketReader : public QObject {
   Q_OBJECT
public:
   // to handle tcp stream
   struct StreamState {
      enum { 
         ReadHeader, ReadPayload 
      }              state;
      std::uint32_t  expected_length;
      std::uint16_t  last_type;
   };

   static constexpr int MAX_PAYLOAD_SIZE = 4096;

public:
   explicit PacketReader(QTcpSocket* socket, QObject* parent = nullptr);

   void ResetStreamState() noexcept;

signals:
   void PacketReceived(std::uint16_t type, const QJsonObject& payload);
private slots:
   void OnRead();

private:
   QTcpSocket* socket_{};
   StreamState stream_state_;
};

// creates a package consisting of binary header and json data
QByteArray CreatePacket(
   std::uint16_t type,
   const QJsonObject&  payload_json
);

} // namespace net

Q_DECLARE_METATYPE(net::Packet);

#endif // PACKET_H_
