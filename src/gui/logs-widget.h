#ifndef LOGS_WIDGET_H_
#define LOGS_WIDGET_H_

#include <QWidget>

namespace gui {

//
// LogsWidget
//
class LogsWidget : public QWidget {
   Q_OBJECT
public:
   LogsWidget(QWidget* parent = nullptr);

public:
   QWidget* GetMainWidget() const noexcept { return main_widget_; }

private:
   QWidget* main_widget_{};
};

} // namespace gui

#endif // LOGS_WIDGET_H_
