#ifndef TABLE_NODE_H_
#define TABLE_NODE_H_

#include <QFrame>
#include <QPushButton>

class QStackedLayout;
class QLabel;

namespace gui {

//
// StatePushButton
//
class StatePushButton : public QPushButton {
   Q_OBJECT
public:
   explicit StatePushButton(QWidget* parent = nullptr);

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

private:
   QLabel* label_addr_{};
   QLabel* label_id_{};
   QLabel* label_status{};
};

//
// TableNode
//
class TableNode : public QFrame {
   Q_OBJECT
public:
   // state false -> inactive
   // state true  -> active
   explicit TableNode(bool state, QWidget *parent = nullptr);

public:
   void SetNodeState(bool new_state) noexcept;

private:
   void DrawNodeData();

private:
   QStackedLayout* 
           main_layout_{};
   ActiveTableNodeData* 
           active_node_{};
   QLabel* inactive_node_{};
   bool    state_{};
};

} // namespace gui

#endif // TABLE_NODE_H_
