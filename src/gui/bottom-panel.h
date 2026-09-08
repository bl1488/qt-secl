#ifndef BOTTOM_PANEL_H_
#define BOTTOM_PANEL_H_

#include <QGroupBox>

class QComboBox;
class QLabel;
class QSpinBox;
class QGridLayout;

namespace net {

struct ServerInfoData;

class  Server;
class  Worker;

} // namespace net

namespace gui {

class ServerWrapper;

//
// BottomPanel
//
class BottomPanel : public QGroupBox {
   Q_OBJECT
public:
   explicit BottomPanel(QWidget* parent = nullptr);

public:
   void Update(const net::ServerInfoData& info);

signals:
   // out signals
   void SpinboxChanged(int rows, int cols);

   // change global state of ServerWrapper
   void WorkerListUpdated(int index);

private:
   void UpdatePages();

   // init table with TableNode wigets
   void InitRowsColsWidgets(QGridLayout* layout);
   // init workers_list_ and workers_info_
   void InitWorkersInfoWidgets(QGridLayout* layout);

private:
   QComboBox* pages_;
   QLabel*    total_clients_;

   QComboBox* workers_list_;
   QLabel*    workers_info_;

   QSpinBox*  rows_spinbox_;
   QSpinBox*  cols_spinbox_;

   int current_worker_index_ = 0;
};

} // namespace gui

#endif // BOTTOM_PANEL_H_
