#include "net/worker.h"
#include "net/net-common.h"
#include "net/packet.h"

#include "include/spdlog-wrapper.h"

#include <QByteArray>
#include <QThread>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>

QString net::ValidateAddrString(const QHostAddress &addr) {
   return (addr.protocol() == QAbstractSocket::IPv4Protocol || addr.toIPv4Address() != 0) ?
      QHostAddress(addr.toIPv4Address()).toString() : 
      addr.toString();
}

//
// SessionWrapper
//
net::SessionWrapper::SessionWrapper(
   std::uint64_t id, 
   QTcpSocket*   socket, 
   QObject*      parent) : QObject(parent), socket_(socket), id_(id) 
{
   assert(socket_);

   packet_reader_ = new PacketReader(socket_, this);
   
   // PacketReader
   connect(packet_reader_, &PacketReader::PacketReceived, this, 
   [this](std::uint16_t type, const QJsonObject& payload){
      ClientInfoData info;
      info.addr  = socket_->peerAddress().toString();
      info.id    = id_;
      info.port  = socket_->peerPort();
      info.state = socket_->state() == QAbstractSocket::ConnectedState;

      emit DataReceivedReady(info, type, payload);
   });

   // disconnected
   connect(socket_, &QTcpSocket::disconnected, this, [this]{
      emit ClientDisconnected(socket_, id_);
   });

   // write
   connect(this, &SessionWrapper::Write, this, &SessionWrapper::OnWrite);
}

void net::SessionWrapper::OnWrite(
   std::uint16_t     type, 
   const QByteArray& payload) 
{
   if (auto state = socket_->state(); state != QAbstractSocket::ConnectedState) {
      GlobalLogDebug("unable to send packet to client {} (state: {})",
         id_, std::uint32_t(state));
      return;
   }

   QJsonObject json;

   switch (type) {
   case Packet::ServerAccept:
      json["value"] = true;
      break;
   case Packet::Text:
      json["value"] = QString(payload);
      break;
   case Packet::Start:
      json["value"] = true;
      break;
   case Packet::Stop:
      json["value"] = false;
      break;
   default:
      GlobalLogError("invalid json type sent by server: {}",
         Packet::TypeToString(type));
      return;
   }

   socket_->write(CreatePacket(type, json));
}

//
// Worker
//
net::Worker::Worker(std::uint16_t worker_id, QObject* parent) : 
   QObject(parent), worker_id_(worker_id)
{
   connect(this, &Worker::Write,      this, &Worker::OnWrite);
   connect(this, &Worker::AddSession, this, &Worker::OnAddSession);
   connect(
      this, 
      &Worker::RequestSessionsSnapshot, 
      this, 
      &Worker::OnRequestSessionsSnapshot
   );

   GlobalLogInfo("worker {} initialized", worker_id_);
}

void net::Worker::OnWrite(
   std::uint64_t     id, 
   std::uint16_t     type,
   const QByteArray& data) const 
{
   auto* client = sessions_list_.value(id);
   if (!client) {
      GlobalLogDebug("{}: no client {} at list", 
         __func__, id); 
      return;
   }
   emit client->Write(type, data);
}

void net::Worker::OnAddSession(qintptr handle) {
   auto* socket = new QTcpSocket(this);
   if (!socket->setSocketDescriptor(handle)) {
      socket->deleteLater();
      return;
   }
   
   std::uint64_t id = GenerateSessionId();
   auto* client     = new SessionWrapper(id, socket, this);
   sessions_list_.insert(id, client);

   sessions_list_counter_.fetch_add(1, std::memory_order_relaxed);

   // client disconnected
   connect(client, &SessionWrapper::ClientDisconnected, this, 
   [this](QTcpSocket* socket, std::uint64_t id) {
      GlobalLogInfo("client [id:{} : addr:{}:{}] disconnected",
         id,
         ValidateAddrString(socket->peerAddress()).toStdString(), 
         socket->peerPort()
      );

      ClientInfoData info;
      info.addr  = socket->peerAddress().toString();
      info.id    = id;
      info.port  = socket->peerPort();
      info.state = socket->state() == QAbstractSocket::ConnectedState;

      emit ClientDisconnected(info);
   });

   connect(client, &SessionWrapper::DataReceivedReady, this, 
   [this](const ClientInfoData& info, std::uint16_t type, const QJsonObject& payload) {
      emit DataReceivedReady(info, type, payload);
   });

   // send accept packet
   emit client->Write(Packet::Type::ServerAccept, {});

   GlobalLogDebug("client was added to worker {}: [id:{} : addr:{}:{}]", 
      worker_id_, 
      id, 
      ValidateAddrString(socket->peerAddress()).toStdString(), 
      socket->peerPort()
   );
}

// can use utils::Random
std::uint64_t net::Worker::GenerateSessionId() const noexcept {
   static std::size_t counter = 0;
   return ++counter;
}

void net::Worker::OnRequestSessionsSnapshot() {
   QList<ClientInfoData> list;
   for (const auto& it : sessions_list_) {
      auto* socket = it->GetSocket();

      ClientInfoData info;
      info.addr  = socket->peerAddress().toString();
      info.port  = socket->peerPort();
      info.id    = it->GetId();
      info.state = socket->state() == QAbstractSocket::ConnectedState;

      list.append(info);
   }
   emit SessionsSnapshotReady(list);
}
