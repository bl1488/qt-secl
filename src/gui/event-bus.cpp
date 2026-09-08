#include "gui/event-bus.h"

#include "net/net-common.h"
#include "net/worker.h"
#include "net/server.h"

//
// EventBus
//
void gui::details::EventBus::Init(net::Server& server) {
   server_ = &server;

   // write
   connect(this, &EventBus::Write, this,
   [this](int worker_index, std::uint64_t id, std::uint16_t type, const QByteArray& payload) {
      if (auto* worker = server_->GetWorker(worker_index))
         emit worker->Write(id, type, payload);
   });

   // request server info
   connect(server_, &net::Server::ServerInfoReady, this,    &EventBus::ServerInfoReady);
   connect(this,    &EventBus::RequestServerInfo,  server_, &net::Server::RequestServerInfo);

   // request worker info
   connect(this, &EventBus::RequestWorkerSnapshot, this, 
   [this](int worker_index) {
      if (auto* worker = server_->GetWorker(worker_index))
         emit worker->RequestSessionsSnapshot();
   });

   for (int i = 0, j = server_->GetWorkersCount(); i < j; ++i) {
      auto* worker = server_->GetWorker(i);
      connect(worker, &net::Worker::SessionsSnapshotReady, this,
      [this, i](const QList<net::ClientInfoData>& list) {
         emit WorkerSnapshotReady(i, list);
      });
   }
}

std::size_t 
gui::details::EventBus::GetSessionsCount(
   int worker_index) const noexcept 
{
   assert(server_ && worker_index < server_->GetWorkersCount());
   return server_->GetWorker(worker_index)->GetSessionsCount();
}

int gui::details::EventBus::GetWorkersCount() const noexcept {
   assert(server_);
   return server_->GetWorkersCount();
}
