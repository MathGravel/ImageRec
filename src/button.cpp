#include "button.h"

Button::Button(QWidget *parent) : QPushButton(parent) {}

void Button::resizeEvent(QResizeEvent *)
{
    QFont f = font();

    if ((width() > 280) && (height() >= 55)) {
        setIconSize(QSize(20,20));
        f.setPointSize(14);
        setFont(f);
    } else {
        setIconSize(QSize(16,16));
        f.setPointSize(10);
        setFont(f);
    }
}
