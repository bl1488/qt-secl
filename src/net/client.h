#ifndef CLIENT_H_
#define CLIENT_H_

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

namespace net {

class Client : public QObject {
   Q_OBJECT
public:
   Client(QObject* parent = nullptr);

   void connect_to_server(const QString& addr, unsigned short port);

private:
   void on_connected();
   void on_read();

public:
   QTcpSocket* socket_{};
};

} // namespace net

#endif // CLIENT_H_
