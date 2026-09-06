#ifndef GUI_MAIN_H_
#define GUI_MAIN_H_

#include "net/server.h"

#include <QMainWindow>

namespace gui {

//
// MainWindow
//
class MainWindow : public QMainWindow {
   Q_OBJECT
public:
   explicit MainWindow(net::Server& server);

private:
   net::Server& server_;
};

} // namespace gui

#endif // GUI_MAIN_H_
