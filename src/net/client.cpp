#include "net/client.h"
#include "include/spdlog-wrapper.h"
#include <qobject.h>

net::Client::Client(QObject* parent) : QObject(parent) {
   socket_ = new QTcpSocket(this);

   connect(socket_, &QTcpSocket::connected, this, [this]{
      on_connected();
   });
   connect(socket_, &QTcpSocket::readyRead, this, [this]{
      on_read();
   });
   connect(socket_, &QTcpSocket::errorOccurred, [this]{
      GlobalLogError("server error: {}", socket_->errorString().toStdString());
   });
}

void net::Client::connect_to_server(const QString& addr, unsigned short port) {
   socket_->connectToHost(addr, port);
}

void net::Client::on_connected() {
   GlobalLogInfo("connected to: {}:{}",
      socket_->peerAddress().toString().toStdString(), socket_->peerPort());
   socket_->write("hello world");
}

void net::Client::on_read() {
   QByteArray buffer = socket_->readAll();
   if (buffer.isEmpty())
      return;

   std::cout << "len: " << buffer.size() << '\n'
             << "msg: " << buffer.data() << '\n';
}
