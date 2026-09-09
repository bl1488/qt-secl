#include "net/server.h"
#include "net/worker.h"
#include "net/net-common.h"

#include "include/spdlog-wrapper.h"

#include <QThread>

//
// Server
//
net::Server::Server(int worker_count, QObject* parent) : 
   QTcpServer(parent)
{
   if (worker_count <= 0)
      throw std::runtime_error("worker_count must be > 0");

   connect(this, &Server::RequestServerInfo, this, &Server::OnRequestServerInfo);

   // init workers
   for (int i = 0; i < worker_count; ++i) {
      QThread* thread = new QThread(this);
      Worker*  worker = new Worker(i + 1);
      worker->moveToThread(thread);

      connect(thread, &QThread::finished, worker, &QThread::deleteLater);
      thread->start();
      
      worker_list_.append(worker);
   }
}

void net::Server::OnRequestServerInfo() {
   ServerInfoData info{};
   for (int i = 0; i < worker_list_.size(); ++i) {
      std::size_t count = worker_list_[i]->GetSessionsCount();
      info.total_clients_count += count;
      info.workers_list.append(worker_list_[i]);
   }
   emit ServerInfoReady(info);
}

bool net::Server::Start(std::uint16_t port) {
   if (!listen(QHostAddress::Any, port)) {
      GlobalLogError("server listening failed: {}", 
         errorString().toStdString());
      return false;
   }
   return true;
}

void net::Server::incomingConnection(qintptr handle) {
   auto* worker = PeekWorker();
   if (!worker) {
      GlobalLogDebug("unable to peek worker");
      return;
   }
   emit worker->AddSession(handle);   
}

std::size_t net::Server::GetWorkerSessionsCount(std::size_t index) const noexcept {
   if (qsizetype(index) <= worker_list_.size())
      return worker_list_[index]->GetSessionsCount();
   return std::size_t(~0ull);
}

// returns the worker with the minimum number of clients
net::Worker* net::Server::PeekWorker() const noexcept {
   // just in case
   if (worker_list_.empty())
      return nullptr;

   auto worker = worker_list_.begin();
   for (auto it = worker + 1; it != worker_list_.end(); ++it) {
      if ((*it)->GetSessionsCount() < (*worker)->GetSessionsCount())
         worker = it;
   }
   return *worker;
}

void net::Server::Stop() {
   auto threads = this->findChildren<QThread*>();
   for (auto* i : threads) {
      if (i->isRunning()) 
         i->quit();
   }
   for (auto* i : threads)
      i->wait();

   GlobalLogInfo("server stopped");
}
