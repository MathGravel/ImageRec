#include "video.h"

Video::Video(QWidget *parent) : QFrame(parent) {}

void Video::resizeEvent(QResizeEvent *)
{
    int heightRatio = (width()/16)*9;
    int widthRatio = (height()/9)*16;

    if (height() != heightRatio) {

        if (height() > heightRatio) {
            resize(width(), heightRatio);
        } else {
            resize(widthRatio, height());
        }
    }

/*
    if ((event->oldSize().width() > 0) && (event->oldSize().height() > 0)) {

        float oldAspectRatio = (float) event->oldSize().width() / event->oldSize().height();
        float newAspectRatio = (float) event->size().width() / event->size().height();

        if (oldAspectRatio != newAspectRatio) {
            if (oldAspectRatio > newAspectRatio) {
                int height = event->oldSize().width()/oldAspectRatio;
                resize(QSize(event->size().width(), height));
            } else {
                int height = event->oldSize().width()/oldAspectRatio;
                resize(QSize(event->size().width(), height));
            }
        }
    }
*/

}
