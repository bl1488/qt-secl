#include "net/client.h"
#include "include/random.h"
#include "include/spdlog-wrapper.h"
#include "net/packet.h"
#include "net/net-common.h"

#include <QTcpSocket>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>
#include <QRandomGenerator>
#include <qabstractsocket.h>

#define GENERATE_DOUBLE_RANDOM(a, b)\
   ((a) + ((b) - (a)) * QRandomGenerator::global()->generateDouble())

//
// Sender
//
net::Sender::Sender(QTcpSocket* socket, QObject* parent) :
   QObject(parent), socket_(socket)
{
   timer_ = new QTimer(this);
   connect(timer_, &QTimer::timeout, this, &Sender::OnTimeout);
}

void net::Sender::StopSending() {
   state_ = false;
   timer_->stop();
}

void net::Sender::StartSending() {
   if (!state_) {
      state_ = true;
      // 10-100 ms
      timer_->start(QRandomGenerator::global()->bounded(10, 100));
   }
}

QByteArray net::Sender::GenerateNetworkMetrics() {
   QJsonObject json;
   json["type"]        = "NetworkMetrics";
   json["bandwidth"]   = GENERATE_DOUBLE_RANDOM(50.0, 100.0);
   json["latency"]     = GENERATE_DOUBLE_RANDOM(10.0, 20.0);
   json["packet_loss"] = QRandomGenerator::global()->generateDouble();

   return CreatePacket(Packet::Text, json);
}

QByteArray net::Sender::GenerateDeviceStatus() {
   QJsonObject json;
   json["type"]         = "DeviceStatus";
   json["uptime"]       = utils::Random(0, 5000);
   json["cpu_usage"]    = utils::Random(0, 100);
   json["memory_usage"] = utils::Random(0, 100);

   return CreatePacket(Packet::Text, json);
}

QByteArray net::Sender::GenerateLog() {
   static constexpr const char* words[] = {
      "hello", "world", "12345", "log",  "test",
      "text",  "1337",  "red",   "blue", "black"
   };
   static constexpr std::size_t words_size = std::size(words);

   // log type
   // short, mid, long
   int log_size;
   switch (utils::Random(1, 3)) {
   case 1: log_size = utils::Random(2, 5);   break;
   case 2: log_size = utils::Random(10, 20); break;
   case 3: log_size = utils::Random(30, 50); break;
   }

   // random log message
   QString message;
   for (int i = 0; i < log_size; ++i)
      message.append(words[utils::Random<int>(0, words_size)]).append(" ");

   QJsonObject json;
   json["type"]     = "Log";
   json["message"]  = message.trimmed();
   json["severity"] = "INFO";

   return CreatePacket(Packet::Text, json);
}

void net::Sender::OnTimeout() {
   if (!state_ || !socket_ || socket_->state() != QAbstractSocket::ConnectedState) {
      emit StopSending();
      return;
   }
   QByteArray packet;
   switch (utils::Random(1, 3)) {
   case 1: packet = GenerateDeviceStatus();   break;
   case 2: packet = GenerateNetworkMetrics(); break;
   case 3: packet = GenerateLog();            break;
   }
   socket_->write(packet);

   // restart in rage [0.01 - 0.1]
   timer_->start(utils::Random(10, 100));
}

//
// Client
//
net::Client::Client(const QString& addr, std::uint16_t port, QObject* parent) : 
   QObject(parent), server_addr_(addr), server_port_(port)
{
   socket_          = new QTcpSocket(this);
   reconnect_timer_ = new QTimer(this);
   packet_reader_   = new PacketReader(socket_, this);
   sender_          = new Sender(socket_, this);

   // sender
   connect(this, &Client::StartSender, this, [this]{ sender_->StartSending(); });
   connect(this, &Client::StopSender,  this, [this]{ sender_->StopSending();  });

   // reconnect timer
   connect(reconnect_timer_, &QTimer::timeout, this, [this]{ Connect(); });

   // connected
   connect(socket_, &QTcpSocket::connected, this, [this]{
      GlobalLogInfo("connected to: {}:{}",
         socket_->peerAddress().toString().toStdString(), 
         socket_->peerPort()
      );
      reconnect_timer_->stop();
   });

   // disconnected
   connect(socket_, &QTcpSocket::disconnected, this, [this]{
      GlobalLogInfo("disconnected from server");

      sender_->StopSending();

      if (!manual_disconnect_ && !reconnect_timer_->isActive()) {
         GlobalLogInfo("trying to reconnect...");
         reconnect_timer_->start(RECONNECT_PERIOD);
      }
      manual_disconnect_ = false;
   });

   // PacketReader read
   connect(packet_reader_, &PacketReader::PacketReceived, this, 
   [this](std::uint16_t type, const QJsonObject& payload) {
      emit DataReceivedReady(type, payload);
   });

   // socket errors
   connect(socket_, &QTcpSocket::errorOccurred, this, &Client::OnSocketError);
}

void net::Client::Connect() {
   // already connected
   auto state = socket_->state();
   if (state == QAbstractSocket::ConnectedState || state == QAbstractSocket::ConnectingState)
      return;
   // recreate
   if (state != QAbstractSocket::UnconnectedState)
      socket_->abort();

   // reset stream state in case of a reconnection
   packet_reader_->ResetStreamState();

   socket_->connectToHost(server_addr_, server_port_);
}

void net::Client::DisconnectManually() {
   manual_disconnect_ = true;
   reconnect_timer_->stop();
   socket_->disconnectFromHost();
}

void net::Client::OnSocketError(QAbstractSocket::SocketError error) {
   GlobalLogError("server error: {}", 
      socket_->errorString().toStdString());
      
   if ((error == QAbstractSocket::ConnectionRefusedError ||
        error == QAbstractSocket::NetworkError           ||
        error == QAbstractSocket::HostNotFoundError)     &&
        !reconnect_timer_->isActive())
   {
      GlobalLogDebug("trying to reconnect...");
      reconnect_timer_->start(RECONNECT_PERIOD);
   }
}
