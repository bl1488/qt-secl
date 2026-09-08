#ifndef SERVER_H_
#define SERVER_H_

#include <QObject>
#include <QTcpServer>

namespace net {

struct ClientInfoData;
struct ServerInfoData;

class Worker;

//
// Server
//
class Server : public QTcpServer {
   Q_OBJECT
public:
   explicit Server(int worker_count, QObject* parent = nullptr);

   ~Server() { Stop(); }

public:
   bool Start(std::uint16_t port);
   void Stop();

   int GetWorkersCount() const noexcept { 
      return worker_list_.size(); 
   }

   std::size_t GetWorkerClientsCount(std::size_t index) const noexcept;

   const Worker* GetWorker(int index) const noexcept {
      return worker_list_.value(index);
   }

   Worker* PeekWorker() const noexcept;

signals:
   void RequestServerInfo();
   void ServerInfoReady(const ServerInfoData& info);
private slots:
   void OnRequestServerInfo();

protected:
   void incomingConnection(qintptr handle) override;

private:
   QList<Worker*> worker_list_;
};

} // namespace net

#endif // SERVER_H_
