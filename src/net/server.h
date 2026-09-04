#ifndef SERVER_H_
#define SERVER_H_

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHash>

#include <atomic>

namespace net {

////////////////////////////////////

struct ServerInfoData {
   int         workers_count;
   std::size_t total_clients_count;
   // pair: worker index : clinets number
   QList<QPair<int, std::size_t>> 
               workers_list;
};

struct ClientInfoData {
   QString       addr;
   std::uint64_t id;
   std::uint16_t port;
   bool          state;
};

////////////////////////////////////

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
   void RequestClientInfoById(std::uint64_t id);
   void RequestClientInfoAll();
   // answer
   void ClientInfoReady(ClientInfoData info);

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

   std::size_t GetWorkerClientsCount(std::size_t index) const noexcept {
      if (qsizetype(index) <= worker_list_.size())
         return worker_list_[index]->GetClientCount();
   }

   Worker* PeekWorker() const noexcept;

public slots:
   void OnServerInfoRequest();
signals:
   void ServerInfoRequestReady(const ServerInfoData& info);

protected:
   void incomingConnection(qintptr handle) override;

private:
   QList<Worker*> worker_list_;
};


} // namespace net

Q_DECLARE_METATYPE(net::ServerInfoData);
Q_DECLARE_METATYPE(net::ClientInfoData);

#endif // SERVER_H_
