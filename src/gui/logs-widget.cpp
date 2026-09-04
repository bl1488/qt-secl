#include "gui/logs-widget.h"
#include "include/spdlog-wrapper.h"

#include <QTextEdit>
#include <QGroupBox>
#include <QVBoxLayout>

gui::LogsWidget::LogsWidget(QWidget* parent) : 
   QWidget(parent) 
{
   auto* widget     = new QWidget();
   auto* layout     = new QVBoxLayout(widget);

   auto* box        = new QGroupBox("logs", widget);
   auto* box_layout = new QVBoxLayout(box);
   auto* text       = new QTextEdit(box);

   text->setReadOnly(true);

   box->setAlignment(Qt::AlignHCenter);

   box_layout->addWidget(text);
   box_layout->setContentsMargins(0, 0, 0, 0);

   layout->setContentsMargins(5, 5, 5, 5);
   layout->addWidget(box);

   GlobalLogDebug("{} init", __func__);
}
