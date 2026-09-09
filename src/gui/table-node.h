#ifndef TABLE_NODE_H_
#define TABLE_NODE_H_

#include <QFrame>
#include <QPushButton>

class QStackedLayout;
class QLabel;
class QLayout;
class QGraphicsOpacityEffect;
class QPropertyAnimation;

namespace net {

struct ClientInfoData;

} // namespace net

namespace gui {

//
// SettingsPopup
//
class SettingsPopup : public QWidget {
   Q_OBJECT
public:
   explicit SettingsPopup(QWidget* parent, const QString &text);

public:
   void Popup(int delay = 2000);

private slots:
   void HideAnimated();

private:
   QGraphicsOpacityEffect* effect_;
   QPropertyAnimation*     animation_;
};

//
// StatePushButton
//
class StatePushButton : public QPushButton {
   Q_OBJECT
public:
   explicit StatePushButton(QWidget* parent = nullptr);

public: 
   bool GetState() const noexcept { return state_; }
   
   bool SetState(bool new_state) noexcept { 
      return state_ = new_state; 
   }

signals:
   // out signal
   void Clicked(bool state);

private:
   void UpdateStyle();

private:
   bool state_{};
};

//
// ActiveTableNodeData
//
class ActiveTableNodeData : public QWidget {
   Q_OBJECT
public:
   explicit ActiveTableNodeData(QWidget* parent = nullptr);

public:
   // update labels info
   void Update(const net::ClientInfoData& info);

private:
   // init horizontal layout with labels:
   // [labelt_session_id_] [label_state_]
   QLayout* InitBottomLayout();
   // init layout with settings, logs, start/stop buttons
   QLayout* InitButtons(QLayout* main_layout);

private:
   QLabel*          label_session_id_;
   QLabel*          label_state_;
   StatePushButton* start_stop_button_;

   int current_worker_index_ = 0;
};

//
// TableNode
//
class TableNode : public QFrame {
   Q_OBJECT
public:
   explicit TableNode(bool state, QWidget *parent = nullptr);

public:
   // active/inactive
   void SetNodeState(bool new_state) noexcept;
   // recall to active_node_->Update()
   void UpdateActiveNodeData(const net::ClientInfoData& info);
   
private:
   QStackedLayout*      main_layout_;
   ActiveTableNodeData* active_node_;
   QLabel*              inactive_node_;
   bool                 state_ = false;
};

} // namespace gui

#endif // TABLE_NODE_H_
