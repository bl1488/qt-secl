#ifndef CLIENT_SESSIONS_WIDGET_H_
#define CLIENT_SESSIONS_WIDGET_H_

#include <QWidget>
#include <QGroupBox>

class QComboBox;
class QHBoxLayout;
class QTimer;
class QVBoxLayout;
class QLabel;
class QTextEdit;
class QSpinBox;

namespace net {

struct ServerInfoData;
class  Server;

} // namespace net

namespace gui {

//
// BottomPanel
//
class BottomPanel : public QGroupBox {
   Q_OBJECT
public:
   BottomPanel(net::Server& server, QWidget* parent = nullptr);

public:
   void UpdateBottomPanel(const net::ServerInfoData& info);

signals:
   void RowsSpinboxChanged(int value);
   void ColsSpinboxChanged(int value);

private:
   void UpdatePages(const net::ServerInfoData& info);

private:
   net::Server& server_;
   QComboBox*   pages_;
   QLabel*      total_clients_{};
   QComboBox*   workers_list_{};
   QLabel*      workers_info_{};

   QSpinBox*    rows_spinbox_{};
   QSpinBox*    cols_spinbox_{};
};

//
// ClientSessionsWidget
//
class ClientSessionsWidget : public QWidget {
   Q_OBJECT
public:
   ClientSessionsWidget(net::Server& server, QWidget* parent = nullptr);

private:
   void InitTimer(QWidget* parent);
   void InitSessionsTable(QWidget* parent);

   void UpdateSessionsTable();

private:
   net::Server& server_;
   QTimer*      timer_{};
   QGroupBox*   sessions_table_{};
   BottomPanel* bottom_panel_{};
};

} // namespace gui

#endif // CLIENT_SESSIONS_WIDGET_H_
