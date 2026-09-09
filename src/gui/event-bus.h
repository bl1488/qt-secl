#ifndef EVENT_BUS_H_
#define EVENT_BUS_H_

#include "net/net-common.h"

#include <QObject>

#define EVENT_BUS_CALL(...)  gui::details::EventBus::Instance()->__VA_ARGS__
#define EVENT_BUS_INSTANCE() gui::details::EventBus::Instance()

namespace net {

class Worker;
class Server;

} // namespace net

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
   // table node buttons
   // start/stop
   void ToggleClientSender (int worker_index, std::uint64_t id, bool state);
   // logs
   void ShowTableNodeLogs  (int worker_index, std::uint64_t id);
   // settings
   void ConfigureSession   (int worker_index, std::uint64_t id);

   // proxy for server
   void RequestServerInfo();
   void ServerInfoReady(const net::ServerInfoData& info);

   // proxy for worker
   void Write(
      int               worker_index, 
      std::uint64_t     sessions_id, 
      std::uint16_t     type, 
      const QByteArray& payload
   );

   void RequestWorkerSnapshot(int worker_index);
   void WorkerSnapshotReady(
      int worker_index, 
      const QList<net::ClientInfoData>& list
   );
   

   // global worker state.
   // some widgets require the current worker index
   void WorkerIndexChanged(int worker_index);

   // arise from worker.
   // logs-widget connecting to this signal
   void GlobalLogStream(
      int                        worker_index, 
      const net::ClientInfoData& info,
      std::uint16_t              type, 
      const QJsonObject&         payload
   );

public:
   int GetWorkersCount() const noexcept;
   std::size_t GetSessionsCount(int worker_index) const noexcept;

private:
   net::Server* server_;
};

} // namespace gui::details

#endif // EVENT_BUS_H_
