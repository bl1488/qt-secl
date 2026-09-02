#ifndef GUI_MAIN_H_
#define GUI_MAIN_H_

#include "net/server.h"

#include <QObject>
#include <QMainWindow>

class QComboBox;
class QHBoxLayout;

namespace gui {

//
// MainWindow
//
class MainWindow : public QMainWindow {
   Q_OBJECT
public:
   constexpr static int ITEMS_PER_PAGE = 12;
   constexpr static int ITEMS_PER_COL  = 3;
public:
   explicit MainWindow(net::Server& server);

private:
   void InitLeftContainer(QHBoxLayout*  main_layout);
   void InitRightContainer(QHBoxLayout* main_layout);

   QWidget* LeftBottomPanel();

   void TimerFunction(QWidget* parent);

private:
   net::Server& server_;
   QTimer*      timer_{};
   QComboBox*   session_page_{};
};

} // namespace gui

#endif // GUI_MAIN_H_
