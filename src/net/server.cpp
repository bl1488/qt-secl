#include "net/server.h"
#include "include/spdlog-wrapper.h"
#include "include/random.h"

#include <QThread>
#include <QByteArray>
#include <QThread>
#include <qabstractsocket.h>
#include <qglobal.h>
#include <qobject.h>
#include <qtcpsocket.h>
#include <qthread.h>
#include <stdexcept>

//
// Server
//
net::Server::Server(int worker_count, QObject* parent) : 
   QTcpServer(parent)
{
   if (worker_count <= 0)
      throw std::runtime_error("worker_count must be > 0");

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

bool net::Server::Start(unsigned short port) {
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
   emit worker->AddClient(handle);   
}

// returns the worker with the minimum number of clients
net::Worker* net::Server::PeekWorker() const noexcept {
   // just in case
   if (worker_list_.empty())
      return nullptr;

   auto worker = worker_list_.begin();
   for (auto it = worker + 1; it != worker_list_.end(); ++it) {
      if ((*it)->GetClientCount() < (*worker)->GetClientCount())
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

   GlobalLogInfo("server is stopped");
}

//
// Worker
//
net::Worker::Worker(std::uint16_t worker_id, QObject* parent) : 
   QObject(parent), worker_id_(worker_id)
{
   connect(this, &Worker::Write,     this, &Worker::DoWrite);
   connect(this, &Worker::AddClient, this, &Worker::DoAddClient);

   GlobalLogInfo("worker {} initialized", worker_id_);
}

void net::Worker::DoWrite(std::uint64_t id, const QString& data) {
   auto* client = client_list_.value(id);
   if (!client) {
      GlobalLogWarning("no client with id {}", id);
      return;
   }
   client->write(data.toUtf8().constData());
}

void net::Worker::DoClientInfoRequest(std::uint64_t id) {
   auto* socket = client_list_.value(id);
   if (!socket) {
      emit ClientInfoReady({});
      return;
   }

   ClientInfo info;
   info.addr  = socket->peerAddress().toString();
   info.state = (socket->state() == QAbstractSocket::ConnectedState);
   info.id    = id;
   info.port  = socket->peerPort();

   emit ClientInfoReady(info);
}

void net::Worker::DoRemoveClient(std::uint64_t id) {
   if (client_list_.remove(id))
      GlobalLogInfo("client {} was removed", id);
}

void net::Worker::DoAddClient(qintptr handle) {
   QTcpSocket* socket = new QTcpSocket(this);
   if (!socket->setSocketDescriptor(handle)) {
      socket->deleteLater();
      return;
   }

   std::uint64_t id = GenerateClientId();
   client_list_.insert(id, socket);

   client_list_counter_.fetch_add(1, std::memory_order_relaxed);
   
   connect(socket, &QTcpSocket::readyRead, this, [this, socket]{ 
      OnRead(socket); 
   });
   connect(socket, &QTcpSocket::disconnected, this, [this, socket]{ 
      OnDisconnected(socket); 
   });

   GlobalLogInfo("client was added to worker {}: [id:{} : addr:{}:{}]", 
      worker_id_, 
      id, 
      socket->peerAddress().toString().toStdString(), 
      socket->peerPort());
}

std::uint64_t net::Worker::GenerateClientId() const noexcept {
   // generate id with xoshiro256
   return utils::Random<std::uint64_t>();
}

void net::Worker::OnRead(QTcpSocket* socket) {
   QByteArray buffer = socket->readAll();
   if (buffer.isEmpty())
      return;
}

void net::Worker::OnDisconnected(QTcpSocket* socket) {
   GlobalLogInfo("client {}:{} disconnected", 
      socket->peerAddress().toString().toStdString(), socket->peerPort());

   client_list_counter_.fetch_sub(1, std::memory_order_relaxed);
}