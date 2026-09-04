#include "gui/gui-main.h"
#include "gui/client-sessions-widget.h"
#include "gui/logs-widget.h"

#include <QHBoxLayout>

gui::MainWindow::MainWindow(net::Server& server) : server_(server) {
   auto* widget = new QWidget(this);
   auto* layout = new QHBoxLayout(widget);

   layout->addWidget(new ClientSessionsWidget(server_, this));
   //layout->addWidget(new LogsWidget(this), 1);

   this->setCentralWidget(widget);
   this->setWindowTitle("server");
}
