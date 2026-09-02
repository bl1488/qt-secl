#include "net/net-common.h"
#include "net/server.h"
#include "gui/gui-main.h"
#include "include/spdlog-wrapper.h"

#include <QApplication>
#include <QtGlobal>

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
   QApplication qapp(argc, argv);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
   qRegisterMetaType<net::ClientInfo>();
#endif

   net::Server server(4);
   if (!server.Start(net::global::PORT)) {
      GlobalLogError("server start failed");
      return 0;
   }
   GlobalLogInfo("server started on port {}", net::global::PORT);

   gui::MainWindow main_wnd(server);
   main_wnd.show();

   return qapp.exec();
}
