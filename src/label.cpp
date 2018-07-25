#include "label.h"

Label::Label(QWidget *parent) : QLabel(parent)
{
    fontSize = font().pointSize();
}

void Label::resizeEvent(QResizeEvent *)
{
    QFont f = font();
    QSize appSize = qApp->topLevelWidgets()[0]->size();

    if ((appSize.width() >= 1250) && (appSize.height() >= 800)) {
        f.setPointSize(fontSize+4);
        setFont(f);
    } else {
        f.setPointSize(fontSize);
        setFont(f);
    }
}
