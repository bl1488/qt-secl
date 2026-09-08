#ifndef CLIENT_H_
#define CLIENT_H_

#include "net/net-common.h"
#include "net/packet.h"

#include <QObject>
#include <QAbstractSocket>

class QTimer;
class QTcpSocket;
class QJsonObject;

namespace net {

//
// Sener
//
class Sender : public QObject {
   Q_OBJECT
public:
   Sender(QTcpSocket* socket, QObject* parent = nullptr);

public:
   void StopSending();
   void StartSending();

private slots:
   void OnTimeout();
private:
   QByteArray GenerateNetworkMetrics();
   QByteArray GenerateLog();
   QByteArray GenerateDeviceStatus();

private:
   QTcpSocket* socket_;
   QTimer*     timer_;
   bool        state_ = false;
};

//
// Client
//
class Client : public QObject {
   Q_OBJECT
public:
   static constexpr int RECONNECT_PERIOD = 5000;
public:
   Client(
      const QString& addr, 
      std::uint16_t  port, 
      QObject*       parent = nullptr
   );

public:
   void Connect();
   void DisconnectManually();
signals:
   void DataReceivedReady(std::uint16_t type, const QJsonObject& payload);

   void StartSender();
   void StopSender();

private slots:
   void OnSocketError(QAbstractSocket::SocketError error);

private:
   QTcpSocket*   socket_;

   QTimer*       reconnect_timer_;
   bool          manual_disconnect_ = false;

   QString       server_addr_;
   std::uint16_t server_port_;

   PacketReader* packet_reader_;

   Sender*       sender_;
};

} // namespace net

#endif // CLIENT_H_
