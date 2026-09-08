#ifndef WORKER_H_
#define WORKER_H_

#include "net/net-common.h"
#include "net/packet.h"

#include <QObject>
#include <QHash>

#include <atomic>

class QTcpSocket;
class QHostAddress;
class QJsonObject;

namespace net {

struct ClientInfoData;
struct ServerInfoData;

QString ValidateAddrString(const QHostAddress& addr);

//
// SessionWrapper
//
class SessionWrapper : public QObject {
   Q_OBJECT
public:
   SessionWrapper(std::uint64_t id, QTcpSocket* socket, QObject* parent = nullptr);

public:
   QTcpSocket* GetSocket() const noexcept { return socket_; }
   std::uint64_t GetId()   const noexcept { return id_;     }

signals:
   void ClientDisconnected(QTcpSocket* socket, std::uint64_t id);

   void DataReceivedReady(
      const ClientInfoData& info, 
      std::uint16_t         type, 
      const QJsonObject&    payload
   );

   void Write(std::uint16_t type, const QByteArray& payload);

private slots:
   void OnWrite(std::uint16_t type, const QByteArray& payload);

private:
   QTcpSocket*   socket_;
   std::uint64_t id_;
   PacketReader* packet_reader_;
};

//
// Worker
//
class Worker : public QObject {
   Q_OBJECT
public:
   Worker(std::uint16_t worker_id, QObject* parent = nullptr);

public:
   std::uint64_t GenerateSessionId() const noexcept;

   std::size_t GetSessionsCount() const noexcept { 
      return sessions_list_counter_.load(std::memory_order_relaxed); 
   }

   std::uint16_t GetWorkerId() const noexcept { return worker_id_; }

signals:
   void AddSession(qintptr handle);
   void Write(std::uint64_t id, std::uint16_t type, const QByteArray& payload) const;

   void DataReceivedReady(
      const ClientInfoData& info, 
      std::uint16_t         type, 
      const QJsonObject&    data
   ) const;

   void ClientDisconnected(const ClientInfoData& info) const;

   void RequestSessionsSnapshot() const;
   void SessionsSnapshotReady(const QList<ClientInfoData>& list) const;

private slots:
   void OnAddSession(qintptr handle);
   void OnWrite(std::uint64_t id, std::uint16_t type, const QByteArray& data) const;
   void OnRequestSessionsSnapshot();

private:
   QHash<std::uint64_t, SessionWrapper*> 
                            sessions_list_;
   std::atomic<std::size_t> sessions_list_counter_{};
   std::uint16_t            worker_id_;
};

} // namespace net

#endif // WORKER_H_
