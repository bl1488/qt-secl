#ifndef GUI_MAIN_H_
#define GUI_MAIN_H_

#include <QMainWindow>

namespace net {

class Server;

} // namespace net

namespace gui {

//
// MainWindow
//
class MainWindow : public QMainWindow {
   Q_OBJECT
public:
   explicit MainWindow(net::Server& server);
};

} // namespace gui

#endif // GUI_MAIN_H_
