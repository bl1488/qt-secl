#include "net/net-common.h"
#include "net/client.h"

#include <QCoreApplication>

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
   QCoreApplication qapp(argc, argv);

   net::Client client;
   client.connect_to_server("127.0.0.1", net::global::PORT);

   return qapp.exec();
}
