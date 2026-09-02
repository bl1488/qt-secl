#include "gui/gui-main.h"
#include "gui/table-node.h"
#include "gui/gui-common.h"
#include "include/spdlog-wrapper.h"
#include "include/random.h"

#include <QTableWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QTimer>
#include <QGridLayout>
#include <QComboBox>
#include <QTextEdit>

void gui::MainWindow::TimerFunction(QWidget* parent) {
   timer_ = new QTimer(parent);
   connect(
      timer_, 
      &QTimer::timeout, 
      this, 
   [this]{
      std::size_t workers_count = server_.GetWorkersCount(), 
            clients_count = 0;
      for (std::size_t i = 0; i < workers_count; ++i) {
         if (auto* worker = server_.GetWorker(i))
            clients_count += worker->GetClientCount();
      }

      int target_pages_count  = (clients_count / ITEMS_PER_PAGE) + 1,
            current_pages_count = session_page_->count();

      if (target_pages_count == current_pages_count) 
         return;

      if (target_pages_count < current_pages_count) {
         for (int i = 0; i < (current_pages_count - target_pages_count); ++i)
            session_page_->removeItem(current_pages_count--);
      }
      else {
         QStringList list;
         for (int i = current_pages_count; i < (target_pages_count - current_pages_count); ++i)
            list.append(QString::number(i + 1));
         session_page_->addItems(list);
      }
   });

   GlobalLogDebug("{} init", __func__);
}

QWidget* gui::MainWindow::LeftBottomPanel() {
   auto* widget = new QWidget();
   auto* layout = new QHBoxLayout(widget);
   layout->setContentsMargins(0, 0, 0, 0); 

   session_page_ = new QComboBox(widget);
   session_page_->setFixedSize(100, 30);

   layout->addWidget(session_page_);
   layout->addStretch(1);

   GlobalLogDebug("{} init", __func__);

   return widget;
}

void gui::MainWindow::InitLeftContainer(QHBoxLayout* main_layout) {
   assert(main_layout);

   auto* widget = new QWidget();
   auto* layout = new QVBoxLayout(widget);

   auto* table  = new QGridLayout();
   auto* box    = new QGroupBox(widget);
   
   table->setSpacing(5);
   table->setAlignment(Qt::AlignTop | Qt::AlignLeft);
   table->setContentsMargins(10, 10, 10, 10);

   for (int i = 0; i < ITEMS_PER_PAGE; ++i) {
      TableNode* node = new TableNode(false, this);
      if (utils::Random<int>(0, 1) == 1)
         node->SetNodeState(true);
      table->addWidget(node, i / ITEMS_PER_COL, i % ITEMS_PER_COL);
   }

   box->setTitle("sessions");
   box->setAlignment(Qt::AlignHCenter);
   box->setLayout(table);
   box->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

   layout->setContentsMargins(5, 5, 5, 5);
   layout->addWidget(box);
   layout->addWidget(LeftBottomPanel());
   layout->addStretch(1);

   TimerFunction(widget);
   timer_->start(1000);

   GlobalLogDebug("{} init", __func__);

   main_layout->addWidget(widget, 0);
}

void gui::MainWindow::InitRightContainer(QHBoxLayout* main_layout) {
   assert(main_layout);

   auto* widget     = new QWidget();
   auto* layout     = new QVBoxLayout(widget);

   auto* box        = new QGroupBox("logs", widget);
   auto* box_layout = new QVBoxLayout(box);
   auto* text       = new QTextEdit(box);

   text->setReadOnly(true);
   text->setText("hello world");

   box->setAlignment(Qt::AlignHCenter);

   box_layout->addWidget(text);
   box_layout->setContentsMargins(0, 0, 0, 0);

   layout->setContentsMargins(5, 5, 5, 5);
   layout->addWidget(box);

   GlobalLogDebug("{} init", __func__);

   main_layout->addWidget(widget, 1);
}

gui::MainWindow::MainWindow(net::Server& server) : server_(server) {
   auto* central_widget = new QWidget(this);
   auto* central_layout = new QHBoxLayout(central_widget);

   InitLeftContainer(central_layout);
   InitRightContainer(central_layout);

   this->setCentralWidget(central_widget);
   this->setWindowTitle("server");
}
