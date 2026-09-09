#include "gui/bottom-panel.h"
#include "gui/gui-common.h"
#include "gui/event-bus.h"

#include "net/server.h"
#include "net/worker.h"
#include "net/net-common.h"

#include "include/spdlog-wrapper.h"

#include <QComboBox>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>

//
// BottomPanel
//
gui::BottomPanel::BottomPanel(QWidget* parent) : QGroupBox(parent) {
   auto* layout   = new QGridLayout(this);

   pages_         = new QComboBox();
   total_clients_ = new QLabel();

   // pages
   pages_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

   // total clients counter
   total_clients_->setAlignment(Qt::AlignCenter);
   total_clients_->setContentsMargins(5, 2, 5, 2);
   total_clients_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
   total_clients_->setObjectName("TotalClientsLabelWidget");
   total_clients_->setStyleSheet(QString(
      "#TotalClientsLabelWidget {"
      "  background-color: #363636;"
      "  border-radius: 3px;"
      "  border: 1px solid %1;"
      "}"
   ).arg(gui::global::BORDER_COLOR_HEX));

   layout->addWidget(new QLabel("page"),          0, 0);
   layout->addWidget(pages_,                      0, 1);
   layout->addWidget(new QLabel("total clients"), 1, 0);
   layout->addWidget(total_clients_,              1, 1);

   // init widgets
   InitWorkersInfoWidgets(layout);
   InitRowsColsWidgets(layout);

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

void gui::BottomPanel::InitWorkersInfoWidgets(QGridLayout* layout) {
   workers_list_  = new QComboBox();
   workers_info_  = new QLabel();

   workers_list_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

   // worker index changed.
   // emit EventBus signal
   connect(workers_list_, qOverload<int>(&QComboBox::currentIndexChanged), this, 
   [this](int index) {
      GlobalLogDebug("{}: worker index changed: {}",
         __func__, current_worker_index_);
      current_worker_index_ = index;

      emit EVENT_BUS_CALL(WorkerIndexChanged(current_worker_index_));
   });

   workers_info_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
   workers_info_->setContentsMargins(5, 2, 5, 2);
   workers_info_->setObjectName("WorkersInfoLabelWidget");
   workers_info_->setStyleSheet(QString(
      "#WorkersInfoLabelWidget {"
      "  background-color: #363636;"
      "  border-radius: 3px;"
      "  border: 1px solid %1;"
      "}"
   ).arg(gui::global::BORDER_COLOR_HEX));

   layout->addWidget(new QLabel("worker"), 2, 0);
   layout->addWidget(workers_list_,        2, 1);
   layout->addWidget(workers_info_,        3, 0, 1, 2);
}

void gui::BottomPanel::InitRowsColsWidgets(QGridLayout* layout) {
   rows_spinbox_  = new QSpinBox();
   cols_spinbox_  = new QSpinBox();

   rows_spinbox_->setValue(3);
   connect(rows_spinbox_, qOverload<int>(&QSpinBox::valueChanged), this, 
   [this](int rows) {
      emit SpinboxChanged(rows, cols_spinbox_->value());
   });

   cols_spinbox_->setValue(3);
   connect(cols_spinbox_, qOverload<int>(&QSpinBox::valueChanged), this, 
   [this](int cols) {
      emit SpinboxChanged(rows_spinbox_->value(), cols);
   });

   layout->addWidget(new QLabel("rows"),  0, 3);
   layout->addWidget(rows_spinbox_,       0, 4);
   layout->addWidget(new QLabel("cols"),  1, 3);
   layout->addWidget(cols_spinbox_,       1, 4);
}

void gui::BottomPanel::Update(const net::ServerInfoData& info) {
   UpdatePages();

   // total clients
   total_clients_->setText(QString::number(info.total_clients_count));
   workers_info_->setText(QString(
      "id:\t%1\n"
      "clients:\t%2"
   ).
   arg(current_worker_index_ + 1).
   arg(EVENT_BUS_CALL(GetSessionsCount(current_worker_index_))));

   // workers list
   for (int i = workers_list_->count(); i < info.workers_list.size(); ++i)
      workers_list_->addItem(QString::number(i + 1));
}

void gui::BottomPanel::UpdatePages() {
   int current_pages_count = pages_->count();
   
   int items_per_page = rows_spinbox_->value() * cols_spinbox_->value();
   if (items_per_page == 0)
      ++items_per_page;
   
   std::size_t sessions_count = EVENT_BUS_CALL(GetSessionsCount(current_worker_index_));

   int target_pages_count = (sessions_count + items_per_page - 1) / items_per_page;
   if (target_pages_count == 0)
      ++target_pages_count;

   // no need to update
   if (target_pages_count == current_pages_count) 
      return;

   if (target_pages_count < current_pages_count) {
      while (pages_->count() > target_pages_count)
         pages_->removeItem(pages_->count() - 1);
   }
   else {
      QStringList list;
      for (int i = current_pages_count; i < target_pages_count; ++i)
         list.append(QString::number(i + 1));
      pages_->addItems(list); 
   }
}
