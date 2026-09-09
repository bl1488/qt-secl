#include "net/net-common.h"
#include "net/server.h"

#include "gui/gui-main.h"

#include "include/spdlog-wrapper.h"

#include <QApplication>
#include <QJsonObject>
#include <QJsonDocument>

int main(int argc, char** argv) {
   QApplication qapp(argc, argv);

   net::Server server(2);
   if (!server.Start(net::global::PORT)) {
      GlobalLogError("server start failed");
      return 0;
   }
   GlobalLogInfo("server started on port {}", net::global::PORT);

   gui::MainWindow main_wnd(server);
   main_wnd.show();

   return qapp.exec();
}
