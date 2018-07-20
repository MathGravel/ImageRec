#include "frame.h"
#include <QDebug>

Frame::Frame(QWidget *parent) : QFrame(parent) {}

void Frame::resizeEvent(QResizeEvent *event)
{
    qDebug() << "=================";
    qDebug() << event->oldSize();
    qDebug() << event->size();
    qDebug() << "=================";

    if ((event->oldSize().width() > 0) && (event->oldSize().height() > 0)) {
        float oldAspectRatio = (float) event->oldSize().width() / event->oldSize().height();
        float newAspectRatio = (float) event->size().width() / event->size().height();

        if (oldAspectRatio != newAspectRatio) {
            if (oldAspectRatio > newAspectRatio) {
                int height = width()*oldAspectRatio;
                resize(QSize(width(), height));
            } else {
                int width = height()*oldAspectRatio;
                resize(QSize(width, height()));
            }
        }
    }
}
