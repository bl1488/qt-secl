#include "gui/table-node.h"
#include "gui/gui-common.h"

#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QStackedLayout>

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
      auto* state       = new StatePushButton(this);
      auto* logs        = new QPushButton("logs", this);
      auto* settings    = new QPushButton(this); 
      
      auto* hbox_layout = new QHBoxLayout();
      auto* layout      = new QVBoxLayout(this);

      hbox_layout->addWidget(logs);
      hbox_layout->addWidget(settings);

      layout->setSpacing(5);
      layout->addWidget(state);
      layout->addLayout(hbox_layout);

      settings->setIcon(QIcon(GetFileAbsolutePath("/rsrc/settings32.png")));
      settings->setStyleSheet(
         "QPushButton {"
         "  background-color: #00558d;"
         "}");

      this->setAttribute(Qt::WA_StyledBackground, true);
      this->setObjectName("ActiveNode");
      this->setStyleSheet(QString(
         "#ActiveNode {"
         "  background-color: #323232;"
         "  border: 2px solid %1;"
         "  border-radius: 10px;"
         "}"
      ).arg(global::BORDER_COLOR_HEX));
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

   DrawNodeData();
}

void gui::TableNode::SetNodeState(bool new_state) noexcept { 
   state_ = new_state;
   DrawNodeData();
}

void gui::TableNode::DrawNodeData() {
   main_layout_->setCurrentIndex(state_ ? 1 : 0);
}
