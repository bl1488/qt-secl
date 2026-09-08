#ifndef EVENT_BUS_H_
#define EVENT_BUS_H_

#include "net/net-common.h"

#include <QObject>

#define EVENT_BUS_CALL(...)  gui::details::EventBus::Instance()->__VA_ARGS__
#define EVENT_BUS_INSTANCE() gui::details::EventBus::Instance()

namespace net {

class Worker;
class Server;

};

namespace gui::details {

//
// EventBus
//
class EventBus : public QObject {
   Q_OBJECT
private:
   EventBus(QObject* parent = nullptr) : QObject(parent) {}

public:
   static EventBus* Instance() {
      static EventBus bus;
      return &bus;
   }

   void Init(net::Server& server_wrapper);

signals:
   // table-node.h
   void ToggleClientSender(bool state, std::uint64_t id);
   void ShowTableNodeLogs(std::uint64_t id);
   void ConfigureSession(std::uint64_t id);

   void Write(
      int               worker_index, 
      std::uint64_t     sessions_id, 
      std::uint16_t     type, 
      const QByteArray& payload
   );

   void RequestServerInfo();
   void ServerInfoReady(const net::ServerInfoData& info);

   void RequestWorkerSnapshot(int worker_index);
   void WorkerSnapshotReady(
      int worker_index, 
      const QList<net::ClientInfoData>& list
   );

public:
   int GetWorkersCount()                          const noexcept;
   std::size_t GetSessionsCount(int worker_index) const noexcept;

private:
   net::Server* server_;
};

} // namespace gui::details

#endif // EVENT_BUS_H_
