#include "gui/table-node.h"
#include "gui/gui-common.h"
#include "gui/event-bus.h"

#include "net/server.h"
#include "net/net-common.h"

#include "include/spdlog-wrapper.h"

#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QStackedLayout>
#include <QGridLayout>
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

//
// SettingsPopup
//
gui::SettingsPopup::SettingsPopup(
   QWidget*       parent, 
   const QString &text) : QWidget(parent) 
{
   QVBoxLayout* layout = new QVBoxLayout(this);

   effect_    = new QGraphicsOpacityEffect(this);
   animation_ = new QPropertyAnimation(effect_, "opacity", this);

   // animation
   animation_->setDuration(250);
   // effect
   effect_ = new QGraphicsOpacityEffect(this);

   QLabel* label = new QLabel(text, this);
   label->setStyleSheet(
      "background-color: #333333; color: white;   "
      "padding: 15px 25px; border-radius: 8px;      "
      "border: 2px solid #02f795; font-size: 13px;"
   );

   layout->setContentsMargins(0, 0, 0, 0);
   layout->addWidget(label);

   this->setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow); 
   this->setAttribute(Qt::WA_TranslucentBackground);
   this->setGraphicsEffect(effect_);
}

void gui::SettingsPopup::Popup(int delay) {
   QWidget* parent = parentWidget();
   if (!parent) 
      return;

   this->adjustSize(); 
   QSize popupSize = sizeHint();

   QPoint pos = parent->rect().center() - QPoint(popupSize.width() / 2, popupSize.height() / 2);
   this->move(pos);

   this->raise();
   this->show();

   animation_->stop();
   animation_->setStartValue(effect_->opacity());
   animation_->setEndValue(1.0);
   animation_->start();

   QTimer::singleShot(delay, this, &SettingsPopup::HideAnimated);
}

void gui::SettingsPopup::HideAnimated() {
   animation_->stop();
   animation_->setStartValue(effect_->opacity());
   animation_->setEndValue(0.0);
   
   disconnect(animation_, &QPropertyAnimation::finished, nullptr, nullptr);
   
   connect(
      animation_, 
      &QPropertyAnimation::finished, 
      this, 
      &SettingsPopup::hide
   );
   connect(
      animation_, 
      &QPropertyAnimation::finished,
      this, 
      &SettingsPopup::deleteLater
   );
   
   animation_->start();
}

//
// StatePushButton
//
gui::StatePushButton::StatePushButton(QWidget* parent) : 
   QPushButton(parent) 
{
   UpdateStyle();
   connect(this, &QPushButton::clicked, this, [this]{
      state_ = !state_;
      UpdateStyle();
      emit Clicked(state_);
   });
}

void gui::StatePushButton::UpdateStyle() {
   if (state_) {
      this->setStyleSheet(
         "QPushButton { border: 1px solid #ff006f; }");
      this->setText("stop");
   }
   else {
      this->setStyleSheet(
         "QPushButton { border: 1px solid #00ff95; }");
      this->setText("start");
   }
}

//
// ActiveTableNodeData
//
gui::ActiveTableNodeData::ActiveTableNodeData(QWidget* parent) : 
   QWidget(parent) 
{
   auto* layout = new QVBoxLayout(this);

   layout->setSpacing(5);

   layout->addLayout(InitButtons(layout));
   layout->addLayout(InitBottomLayout());

   this->setAttribute(Qt::WA_StyledBackground, true);
   this->setObjectName("ActiveNodeWidget");
   this->setStyleSheet(QString(
      "#ActiveNodeWidget {"
      "  background-color: #323232;"
      "  border: 2px solid %1;"
      "  border-radius: 10px;"
      "}"
   ).arg(global::BORDER_COLOR_HEX));
}

QLayout* gui::ActiveTableNodeData::InitBottomLayout() {
   auto* layout      = new QGridLayout();

   label_session_id_ = new QLabel();
   label_state_      = new QLabel();

   auto style = QString(
      "QLabel {"
      "  background-color: #363636;"
      "  border-radius: 3px;"
      "  border: 1px solid %1;"
      "}"
   ).arg(gui::global::BORDER_COLOR_HEX);

   // id
   label_session_id_->setAlignment(Qt::AlignCenter);
   label_session_id_->setStyleSheet(style);
   // state
   label_state_->setAlignment(Qt::AlignCenter);
   label_state_->setStyleSheet(style);

   layout->addWidget(label_session_id_, 0, 0);
   layout->addWidget(label_state_,      0, 1);

   return layout;
}

QLayout* gui::ActiveTableNodeData::InitButtons(QLayout* main_layout) {
   auto* layout       = new QHBoxLayout();

   start_stop_button_ = new StatePushButton();
   auto* settings     = new QPushButton(); 
   auto* logs         = new QPushButton("logs");

   settings->setIcon(QIcon(GetFileAbsolutePath("/rsrc/settings32.png")));
   settings->setStyleSheet(
      "QPushButton {"
      "  background-color: #00558d;"
      "}"
   );
   
   // EventBus
   connect(settings, &QPushButton::clicked, this, [this]() {
      GlobalLogDebug("worder id {}: settings button clicked", 
         current_worker_index_ + 1
      );

      auto* popup = new SettingsPopup(
         this->window(), "coming soon...");
      popup->Popup(3000); 

      std::uint64_t id = label_session_id_->text().toULongLong();
      emit EVENT_BUS_CALL(ConfigureSession(current_worker_index_, id));
   });
   connect(logs, &QPushButton::clicked, this, [this]() {
      GlobalLogDebug("worker id {}: logs button clicked", 
         current_worker_index_ + 1
      );
      std::uint64_t id = label_session_id_->text().toULongLong();
      emit EVENT_BUS_CALL(ShowTableNodeLogs(current_worker_index_, id));
   });
   connect(start_stop_button_, &StatePushButton::Clicked, [this](bool state) {
      GlobalLogDebug("worker id {}: start/stop button clicked: {}", 
         current_worker_index_ + 1, state ? "start" : "stop"
      );
      std::uint64_t id = label_session_id_->text().toULongLong();
      emit EVENT_BUS_CALL(ToggleClientSender(current_worker_index_, id, state));
   });

   connect(EVENT_BUS_INSTANCE(), &details::EventBus::WorkerIndexChanged, this,
   [this](int worker_index) {
      current_worker_index_ = worker_index;
   });

   main_layout->addWidget(start_stop_button_);

   layout->addWidget(logs);
   layout->addWidget(settings);

   return layout;
}

void gui::ActiveTableNodeData::Update(const net::ClientInfoData& info) {
   label_session_id_->setText(QString::number(info.id));
   label_state_->setText(info.state ? 
      "<b><font color='green'>online</font></b>" : "<b><font color='red'>offline</font></b>"
   );
}

//
// TableNode
//
gui::TableNode::TableNode(bool state, QWidget *parent) : 
   QFrame(parent), state_(state) 
{
   this->setFixedSize(130, 100);

   active_node_   = new ActiveTableNodeData(this);
   inactive_node_ = new QLabel("empty", this);

   main_layout_   = new QStackedLayout(this);

   inactive_node_->setAlignment(Qt::AlignCenter);
   inactive_node_->setStyleSheet(QString(
      "QLabel {"
      "  border: 2px dashed %1;"
      "  border-radius: 10px;"
      "}"
   ).arg(global::BORDER_COLOR_HEX));

   main_layout_->addWidget(inactive_node_);
   main_layout_->addWidget(active_node_);

   main_layout_->setCurrentIndex(state_ ? 1 : 0);
}

void gui::TableNode::SetNodeState(bool new_state) noexcept { 
   state_ = new_state;
   main_layout_->setCurrentIndex(state_ ? 1 : 0);
}

void gui::TableNode::UpdateActiveNodeData(const net::ClientInfoData& info) {
   active_node_->Update(info);
}
