#include "gui/gui-main.h"
#include "gui/event-bus.h"
#include "gui/sessions-widget.h"
#include "gui/event-bus.h"
#include "gui/logs-widget.h"

#include "net/server.h"

#include <QHBoxLayout>

//
// MainWindow
//
gui::MainWindow::MainWindow(net::Server& server) {
   auto* widget = new QWidget(this);
   auto* layout = new QHBoxLayout(widget);

   // init event bus
   details::EventBus::Instance()->Init(server);

   layout->addWidget(new ClientSessionsWidget(this));
   layout->addWidget(new LogsWidget(this), 1);

   this->setCentralWidget(widget);
   this->setWindowTitle("server");
}
