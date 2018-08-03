#include "label.h"

Label::Label(QWidget *parent) : QLabel(parent) {}

void Label::mousePressEvent(QMouseEvent*)
{
    emit clicked();
}
