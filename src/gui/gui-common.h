#ifndef GUI_COMMON_H_
#define GUI_COMMON_H_

#include <QString>
#include <QFileInfo>
#include <QColor>

namespace gui {

inline QString GetFileAbsolutePath(const QString& additional) {
   return QFileInfo(__FILE__).absolutePath() + additional;
}

namespace global {

constexpr const char* BORDER_COLOR_HEX = "#4b4b4b";

} // namespace gui::global

} // namespace gui

#endif // GUI_COMMON_H_
