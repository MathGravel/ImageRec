#include "video.h"

Video::Video(QWidget *parent) : QFrame(parent) {}

void Video::resizeEvent(QResizeEvent *)
{
    int heightRatio = (width()/16)*9;
    int widthtRatio = (height()/9)*16;

    if (height() != heightRatio) {
        if (height() > heightRatio) {
            resize(QSize(width(), heightRatio));
        } else {
            resize(QSize(widthtRatio, height()));
        }
    }
}
