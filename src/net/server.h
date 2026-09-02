#ifndef SERVER_H_
#define SERVER_H_

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHash>

#include <atomic>

namespace net {

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
   bool Start(unsigned short port);
   void Stop();

   std::size_t GetWorkersCount() 
      const noexcept { return worker_list_.size(); }

   Worker* GetWorker(std::size_t index) 
      const noexcept { return worker_list_.at(index); }

   Worker* PeekWorker() const noexcept;

protected:
   void incomingConnection(qintptr handle) override;

private:
   QList<Worker*> worker_list_;
};

struct ClientInfo {
   QString       addr;
   std::uint64_t id;
   std::uint16_t port;
   bool          state;
};

//
// Worker
//
class Worker : public QObject {
   Q_OBJECT
public:
   Worker(std::uint16_t worker_id, QObject* parent = nullptr);

public:
   std::uint64_t GenerateClientId() const noexcept;

   std::size_t GetClientCount() const noexcept {
      return client_list_counter_.load(std::memory_order_relaxed);
   }

   std::uint16_t GetWorkerId() const noexcept { return worker_id_; }

signals:
   void Write(std::uint64_t id, const QString& data);
   void AddClient(qintptr handle);
   void RemoveClient(std::uint64_t id);

   // signals for obtaining info about client.
   // request
   void RequestClientInfo(std::uint64_t id);
   void RequestClientInfoAll();
   // answer
   void ClientInfoReady(ClientInfo info);

private slots:
   void DoWrite(std::uint64_t id, const QString& data);
   void DoAddClient(qintptr handle);
   void DoRemoveClient(std::uint64_t id);
   void DoClientInfoRequest(std::uint64_t id);
private:
   void OnRead(QTcpSocket* socket);
   void OnDisconnected(QTcpSocket* socket);

private:
   QHash<std::uint64_t, QTcpSocket*> 
                            client_list_;
   std::atomic<std::size_t> client_list_counter_{};
   std::uint16_t            worker_id_{};
};

} // namespace net

Q_DECLARE_METATYPE(net::ClientInfo);

#endif // SERVER_H_
