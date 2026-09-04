#include "gui/client-sessions-widget.h"
#include "gui/gui-common.h"
#include "gui/table-node.h"
#include "net/server.h"
#include "include/spdlog-wrapper.h"

#include <QWidget>
#include <QTextEdit>
#include <QComboBox>
#include <QGroupBox>
#include <QTimer>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QStandardItemModel>
#include <QTreeView>
#include <QHeaderView>
#include <QLabel>
#include <qcombobox.h>
#include <QSpinBox>
#include <qglobal.h>
#include <qgridlayout.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qnamespace.h>
#include <qsize.h>
#include <qsizepolicy.h>
#include <qspinbox.h>
#include <qtextedit.h>

namespace global {

// vars for managing client session table
int ITEMS_PER_ROW = 2;
int ITEMS_PER_COL = 3;

// timer delay for network module polling
int TIMER_TIMEOUT  = 1000;

} // namespace global

//
// BottomPanel
//
gui::BottomPanel::BottomPanel(net::Server& server, QWidget* parent) :
   QGroupBox(parent), server_(server) 
{
   auto* layout   = new QGridLayout(this);

   pages_         = new QComboBox();
   total_clients_ = new QLabel();
   workers_list_  = new QComboBox();
   workers_info_  = new QLabel();

   rows_spinbox_  = new QSpinBox();
   cols_spinbox_  = new QSpinBox();

   // pages
   pages_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

   // workers list
   workers_list_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

   // clients counter
   total_clients_->setAlignment(Qt::AlignCenter);
   total_clients_->setContentsMargins(5, 2, 5, 2);
   total_clients_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
   total_clients_->setObjectName("TotalClientsLabel");
   total_clients_->setStyleSheet(QString(
      "#TotalClientsLabel {"
      "  background-color: #363636;"
      "  border-radius: 3px;"
      "  border: 1px solid %1;"
      "}"
   ).arg(gui::global::BORDER_COLOR_HEX));

   // workers info label
   workers_info_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
   workers_info_->setContentsMargins(5, 2, 5, 2);
   workers_info_->setObjectName("WorkersInfoLabel");
   workers_info_->setStyleSheet(QString(
      "#WorkersInfoLabel {"
      "  background-color: #363636;"
      "  border-radius: 3px;"
      "  border: 1px solid %1;"
      "}"
   ).arg(gui::global::BORDER_COLOR_HEX));

   connect(workers_list_, &QComboBox::currentIndexChanged, this, 
   [this](int index) {
      workers_info_->setText(QString(
         "id:\t%1\n"
         "clients:\t%2"
      ).arg(index + 1).arg(server_.GetWorkerClientsCount(index)));
   });

   layout->addWidget(new QLabel("page"),            0, 0);
   layout->addWidget(pages_,                        0, 1);
   layout->addWidget(new QLabel("total clients"),   1, 0);
   layout->addWidget(total_clients_,                1, 1);
   layout->addWidget(new QLabel("worker"),          2, 0);
   layout->addWidget(workers_list_,                 2, 1);
   layout->addWidget(workers_info_,                 3, 0);

   // rows
   rows_spinbox_->setValue(::global::ITEMS_PER_ROW);
   connect(
      rows_spinbox_, 
      qOverload<int>(&QSpinBox::valueChanged), 
      this, 
      &BottomPanel::RowsSpinboxChanged
   );

   // cols
   cols_spinbox_->setValue(::global::ITEMS_PER_COL);
   connect(
      cols_spinbox_, 
      qOverload<int>(&QSpinBox::valueChanged), 
      this, 
      &BottomPanel::RowsSpinboxChanged
   );

   layout->addWidget(new QLabel("rows"),            0, 3);
   layout->addWidget(rows_spinbox_,                 0, 4);
   layout->addWidget(new QLabel("cols"),            1, 3);
   layout->addWidget(cols_spinbox_,                 1, 4);

   layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
   layout->setContentsMargins(10,10,10,10);

   this->setObjectName("BottomPanelWidget");
   this->setStyleSheet(QString(
      "#BottomPanelWidget {"
      "  border: 1px solid %1;"
      "}"
   ).arg(gui::global::BORDER_COLOR_HEX));

   GlobalLogDebug("initialized: {}()", __func__);
}

void gui::BottomPanel::UpdateBottomPanel(const net::ServerInfoData& info) {
   UpdatePages(info);

   // total clients
   total_clients_->setText(QString::number(info.total_clients_count));

   // workers list
   for (int i = workers_list_->count(); i < info.workers_count; ++i)
      workers_list_->addItem(QString::number(i + 1));
}

void gui::BottomPanel::UpdatePages(const net::ServerInfoData& info) {
   int target_pages_count  =
         (info.total_clients_count / (::global::ITEMS_PER_ROW * ::global::ITEMS_PER_ROW)) + 1;
   int current_pages_count = pages_->count();

   // no need to update
   if (target_pages_count == current_pages_count) 
      return;

   if (target_pages_count < current_pages_count) {
      for (int i = 0; i < (current_pages_count - target_pages_count); ++i)
         pages_->removeItem(current_pages_count--);
   }
   else {
      QStringList list;
      for (int i = current_pages_count; i < (target_pages_count - current_pages_count); ++i)
         list.append(QString::number(i + 1));
      pages_->addItems(list);
   }
}

//
// ClientSessionsWidget
//
void gui::ClientSessionsWidget::InitSessionsTable(QWidget* parent) {
   sessions_table_ = new QGroupBox(parent);
   auto* layout    = new QGridLayout();
   
   layout->setSpacing(5);
   layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
   layout->setContentsMargins(10, 10, 10, 10);

   sessions_table_->setLayout(layout);
   sessions_table_->setObjectName("SessionsLabelWidget");
   sessions_table_->setStyleSheet(QString(
      "#SessionsLabelWidget {"
      "  border: 1px solid %1;"
      "}"
   ).arg(gui::global::BORDER_COLOR_HEX));

   GlobalLogDebug("initialized: {}()", __func__);
}

gui::ClientSessionsWidget::ClientSessionsWidget(net::Server& server, QWidget* parent) : 
   QWidget(parent), server_(server)
{
   auto* layout = new QVBoxLayout(this);

   // init session
   InitSessionsTable(this);

   layout->addWidget(sessions_table_);
   layout->addWidget((bottom_panel_ = new BottomPanel(server_)));
   
   layout->setContentsMargins(5, 5, 5, 5);
   layout->addStretch(1);

   // timer
   InitTimer(this);
   timer_->start(1000);

   GlobalLogDebug("initialized: {}()", __func__);
}

void gui::ClientSessionsWidget::InitTimer(QWidget* parent) {
   timer_ = new QTimer(parent);
   
   connect(&server_, &net::Server::ServerInfoRequestReady, this, 
   [this](const net::ServerInfoData& info) {
      bottom_panel_->UpdateBottomPanel(info);
   });

   connect(timer_, &QTimer::timeout, this, [this]{
      // change
      auto* layout = reinterpret_cast<QGridLayout*>(sessions_table_->layout());
      for (int i = 0; i < ::global::ITEMS_PER_ROW * ::global::ITEMS_PER_COL; ++i) {
         layout->addWidget(
            new TableNode(false, sessions_table_), 
            i / ::global::ITEMS_PER_COL, i % ::global::ITEMS_PER_COL
         );
      }
      QMetaObject::invokeMethod(
         &server_, 
         &net::Server::OnServerInfoRequest, 
         Qt::QueuedConnection
      );
   });

   GlobalLogDebug("initialized: {}()", __func__);
}
