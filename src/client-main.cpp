#include "include/spdlog-wrapper.h"

#include "net/net-common.h"
#include "net/client.h"

#include <QCoreApplication>
#include <QTimer>
#include <QJsonObject>

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
   QCoreApplication qapp(argc, argv);

   net::Client client("localhost", net::global::PORT);
   client.Connect();

   QObject::connect(&client, &net::Client::DataReceivedReady, 
   [&client](std::uint16_t type, const QJsonObject& payload) {
      if (type == net::Packet::ServerAccept) {
         if (payload.contains("value") && payload.value("value").toBool() == true) {
            GlobalLogInfo("{}: server successfully accept: ", 
               net::Packet::TypeToString(type));
         }
         else {
            GlobalLogError("{}: rejected", net::Packet::TypeToString(type));
            client.DisconnectManually();
         }
      }
      else if (type == net::Packet::Start) {
         GlobalLogInfo("{}: {}", 
            net::Packet::TypeToString(type), 
            payload.value("value").toBool()
         );
         emit client.StartSender();
      }
      else if (type == net::Packet::Stop) {
         GlobalLogInfo("{}: {}", 
            net::Packet::TypeToString(type), 
            payload.value("value").toBool()
         );
         emit client.StopSender();
      }
   });

   return qapp.exec();
}
