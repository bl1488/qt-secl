#include "net/net-common.h"
#include "net/server.h"

#include "gui/gui-main.h"

#include "include/spdlog-wrapper.h"

#include <QApplication>
#include <QJsonObject>
#include <QStyleFactory>
#include <QJsonDocument>

int main(int argc, char** argv) {
   QApplication qapp(argc, argv);

   QApplication::setStyle(QStyleFactory::create("Fusion"));

   QPalette darkPalette;
   darkPalette.setColor(QPalette::Window,          QColor(53, 53, 53));
   darkPalette.setColor(QPalette::WindowText,      Qt::white);
   darkPalette.setColor(QPalette::Base,            QColor(42, 42, 42));
   darkPalette.setColor(QPalette::AlternateBase,   QColor(66, 66, 66));
   darkPalette.setColor(QPalette::ToolTipBase,     Qt::white);
   darkPalette.setColor(QPalette::ToolTipText,     Qt::white);
   darkPalette.setColor(QPalette::Text,            Qt::white);
   darkPalette.setColor(QPalette::Button,          QColor(53, 53, 53));
   darkPalette.setColor(QPalette::ButtonText,      Qt::white);
   darkPalette.setColor(QPalette::BrightText,      Qt::red);
   darkPalette.setColor(QPalette::Link,            QColor(42, 130, 218));
   
   darkPalette.setColor(QPalette::Highlight,       QColor(42, 130, 218));
   darkPalette.setColor(QPalette::HighlightedText, Qt::black);

   darkPalette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(127, 127, 127));
   darkPalette.setColor(QPalette::Disabled, QPalette::Text,       QColor(127, 127, 127));
   darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(127, 127, 127));

   QApplication::setPalette(darkPalette);

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
