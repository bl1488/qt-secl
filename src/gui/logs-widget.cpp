#include "gui/logs-widget.h"
#include "gui/gui-common.h"
#include "gui/event-bus.h"

#include "net/server.h"
#include "net/packet.h"
#include "net/worker.h"
#include "net/net-common.h"

#include "include/spdlog-wrapper.h"

#include <QTextEdit>
#include <QJsonObject>
#include <QJsonDocument>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>

gui::LogsWidget::LogsWidget(QWidget* parent) : 
   QGroupBox(parent) 
{
   auto* layout   = new QVBoxLayout(this);

   message_types_ = new QComboBox();
   text_          = new QTextEdit();
   session_info_  = new QLabel("empty");

   connect(this, &LogsWidget::Update, this, &LogsWidget::OnUpdate);

   connect(EVENT_BUS_INSTANCE(), &details::EventBus::GlobalLogStream, this,
   [this](
      [[maybe_unused]] int       worker_index, 
      const net::ClientInfoData& info, 
      std::uint16_t              type,
      const QJsonObject&         payload) 
   {
      if (worker_index == current_worker_index_)
         OnUpdate(info, type, payload);
   });

   connect(EVENT_BUS_INSTANCE(), &details::EventBus::WorkerIndexChanged, this, 
   [this](int worker_index) { 
      current_worker_index_ = worker_index;
   });

   // text
   text_->setReadOnly(true);
   text_->setStyleSheet(QString(
      "QTextEdit {"
      "  border: 1px solid %1"
      "}"
   ).arg(global::BORDER_COLOR_HEX));

   // sessions info
   session_info_->setStyleSheet(QString(
      "QLabel {"
      "  background-color: #363636;"
      "  border-radius: 3px;"
      "  border: 1px solid %1;"
      "}"
   ).arg(gui::global::BORDER_COLOR_HEX));
   session_info_->setAlignment(Qt::AlignLeft);

   // top layout
   auto* top_layout   = new QHBoxLayout();

   auto* clear_button = new QPushButton("clear");
   // clear text widget
   connect(clear_button, &QPushButton::clicked, this, [this] {
      text_->clear();
   });

   message_types_->addItems({ 
      "NetworkMetrics", "DeviceStatus", "Log"
   });

   top_layout->addWidget(new QLabel("message type"));
   top_layout->addWidget(message_types_);
   top_layout->addWidget(clear_button);
   top_layout->addStretch();

   connect(message_types_, &QComboBox::currentIndexChanged, this, [this](int index){
      text_->clear();
   });

   // adding widgets
   layout->addLayout(top_layout);
   layout->addWidget(session_info_);
   layout->addWidget(text_);

   this->setLayout(layout);
   this->setObjectName("MainLogsWidget");
   this->setStyleSheet(QString(
      "#MainLogsWidget {"
      "  border: 1px solid %1;"
      "}"
   ).arg(global::BORDER_COLOR_HEX));

   GlobalLogDebug("initialized: {}()", __func__);
}

void gui::LogsWidget::OnUpdate(
   const net::ClientInfoData& info, 
   std::uint16_t              type, 
   const QJsonObject&         payload)
{
   if (payload.contains("type") && 
       payload.value("type").toString() == message_types_->currentText())
   {
      QByteArray data = QJsonDocument(payload).toJson(QJsonDocument::Indented);
      text_->append(QString(
         "type:%1\n"
         "payload:\n%2\n"
         "====================================\n"
      )
      .arg(net::Packet::TypeToString(type))
      .arg(data));
   }

   session_info_->setText(QString(
      "id:\t%1\n"
      "addr:\t%2:%3\n"
      "status:\t%4"
   )
   .arg(info.id)
   .arg(net::ValidateAddrString(QHostAddress(info.addr)))
   .arg(info.port)
   .arg(info.state ? "online" : "offline"));
}
