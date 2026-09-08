#include "gui/sessions-widget.h"
#include "gui/gui-common.h"
#include "gui/event-bus.h"
#include "gui/table-node.h"
#include "gui/bottom-panel.h"

#include "net/client.h"
#include "net/server.h"
#include "net/worker.h"

#include "include/spdlog-wrapper.h"

#include <QGroupBox>
#include <QTimer>
#include <QGridLayout>

//
// ClientSessionsWidget
//
gui::ClientSessionsWidget::ClientSessionsWidget(QWidget* parent) :
   QWidget(parent) 
{
   auto* layout  = new QVBoxLayout(this);

   // init bottom panel
   bottom_panel_ = new BottomPanel(this);
   // init table
   InitSessionsTable(this);

   layout->addWidget(sessions_table_);
   layout->addWidget(bottom_panel_);
   
   layout->setContentsMargins(0, 0, 0, 0);
   layout->addStretch(1);

   // init timer
   InitTimer(this);
   timer_->start(TIMER_PERIOD);

   GlobalLogDebug("initialized: {}()", __func__);
}

void gui::ClientSessionsWidget::InitTimer(QWidget* parent) {
   timer_ = new QTimer(parent);

   // workers
   connect(EVENT_BUS_INSTANCE(), &details::EventBus::WorkerSnapshotReady, this, 
   [this](int worker_index, const QList<net::ClientInfoData>& list) {
      if (worker_index == current_worker_index_)
         UpdateTableNodes(list);
   });

   // server
   connect(EVENT_BUS_INSTANCE(), &details::EventBus::ServerInfoReady, this, 
   [this](const net::ServerInfoData& info) {
      emit bottom_panel_->Update(info);
   });

   // timer
   connect(timer_, &QTimer::timeout, this, [this] {
      emit EVENT_BUS_CALL(RequestServerInfo());
      emit EVENT_BUS_CALL(RequestWorkerSnapshot(current_worker_index_));
   });
}

void gui::ClientSessionsWidget::InitSessionsTable(QWidget* parent) {
   assert(bottom_panel_);

   auto* layout    = new QGridLayout();

   sessions_table_ = new QGroupBox(parent);
   
   // sessions table
   sessions_table_->setLayout(layout);
   sessions_table_->setObjectName("SessionsLabelWidget");
   sessions_table_->setStyleSheet(QString(
      "#SessionsLabelWidget {"
      "  border: 1px solid %1;"
      "}"
   ).arg(gui::global::BORDER_COLOR_HEX));

   UpdateTable(3, 3);

   // update table (rows, cols)
   connect(bottom_panel_, &BottomPanel::SpinboxChanged, this, 
   [this](int rows, int cols) {
      UpdateTable(rows, cols);
   });
   // update current worker index
   connect(bottom_panel_, &BottomPanel::WorkerListUpdated, this, 
   [this](int index) { 
      current_worker_index_ = index;
   });

   layout->setSpacing(5);
   layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
   layout->setContentsMargins(10, 10, 10, 10);

   GlobalLogDebug("initialized: {}()", __func__);
}

void gui::ClientSessionsWidget::UpdateTable(int rows, int cols) {
   auto* layout = static_cast<QGridLayout*>(sessions_table_->layout());

   QList<QWidget*> nodes_list;
   while (layout->count()) {
      QLayoutItem* item = layout->takeAt(0);
      if (item) {
         if (QWidget* w = item->widget())
            nodes_list.append(w);
         delete item;
      }
   }
   int total_nodes   = rows * cols;
   int current_nodes = nodes_list.size();

   // distribute nodes
   for (int i = 0; i < current_nodes; ++i) {
      if (i < total_nodes)
         layout->addWidget(nodes_list[i], i / cols, i % cols);
      else nodes_list[i]->deleteLater();
   }
   // add new nodes
   for (int i = current_nodes; i < total_nodes; ++i)
      layout->addWidget(new TableNode(false, sessions_table_), i / cols, i % cols);

   if (QWidget* main_wnd = this->window())
      main_wnd->resize(main_wnd->sizeHint());
}

void gui::ClientSessionsWidget::UpdateTableNodes(
   const QList<net::ClientInfoData>& list) 
{
   auto* layout = static_cast<QGridLayout*>(sessions_table_->layout());
   for (int i = 0, j = layout->count(); i < j; ++i) {
      if (TableNode* node = reinterpret_cast<TableNode*>(layout->itemAt(i)->widget()); node) {
         if (i < list.size()) {
            node->SetNodeState(true);
            node->UpdateActiveNodeData(list[i]);
         }
         else node->SetNodeState(false);
      }
   }
}

