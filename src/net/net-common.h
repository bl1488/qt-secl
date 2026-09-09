#ifndef NET_COMMON_H
#define NET_COMMON_H

#include <QMetaType>
#include <QPair>
#include <QList>

namespace net {

namespace global {

constexpr std::uint16_t PORT = 12345;

} // namespace net::global

class Worker;

////////////////////////////////////

struct ServerInfoData {
   std::size_t    total_clients_count;
   QList<Worker*> workers_list;
};

struct ClientInfoData {
   QString       addr;
   std::uint64_t id;
   std::uint16_t port;
   bool          state;
};

////////////////////////////////////

} // namespace net

Q_DECLARE_METATYPE(net::ServerInfoData);
Q_DECLARE_METATYPE(net::ClientInfoData);

#endif // NET_COMMON_H
