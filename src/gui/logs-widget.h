#ifndef LOGS_WIDGET_H_
#define LOGS_WIDGET_H_

#include <QWidget>
#include <QGroupBox>

class QComboBox;
class QTextEdit;
class QJsonObject;
class QLabel;

namespace net {

struct ClientInfoData;

} // namespace net

namespace gui {

//
// LogsWidget
//
class LogsWidget : public QGroupBox {
   Q_OBJECT
public:
   explicit LogsWidget(QWidget* parent = nullptr);

signals:
   void Update(
      const net::ClientInfoData& info, 
      std::uint16_t              type, 
      const QJsonObject&         payload
   );
private slots:
   void OnUpdate(
      const net::ClientInfoData& info, 
      std::uint16_t              type, 
      const QJsonObject&         payload
   );

private:
   QComboBox* message_types_;
   QTextEdit* text_;
   QLabel*    session_info_;
};

} // namespace gui

#endif // LOGS_WIDGET_H_
