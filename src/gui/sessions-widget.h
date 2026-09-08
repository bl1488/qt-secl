#ifndef CLIENT_SESSIONS_WIDGET_H_
#define CLIENT_SESSIONS_WIDGET_H_

#include <QWidget>

class QTimer;
class QGroupBox;

namespace net {

class  SessionWrapper;
struct ClientInfoData;

} // namespace net

namespace gui {

class ServerWrapper;

// src: bottom-panel.h
class BottomPanel;

//
// ClientSessionsWidget
//
class ClientSessionsWidget : public QWidget {
   Q_OBJECT
public:
   static constexpr int TIMER_PERIOD = 100;
public:
   explicit ClientSessionsWidget(QWidget* parent = nullptr);

private:
   void InitSessionsTable(QWidget* parent);
   void InitTimer(QWidget* parent);

   void UpdateTable(int rows, int cols);
   void UpdateTableNodes(const QList<net::ClientInfoData>& list);

private:
   QTimer*      timer_;
   QGroupBox*   sessions_table_;
   BottomPanel* bottom_panel_;

   int current_worker_index_ = 0;
};

} // namespace gui

#endif // CLIENT_SESSIONS_WIDGET_H_
