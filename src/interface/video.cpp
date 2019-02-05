#include "video.h"

Video::Video(QWidget *parent) : QLabel(parent)
{
  this->parent = parent;
  this->setMinimumSize(1,1);
  setScaledContents(false);
}

void Video::setPixmap(const QPixmap &p)
{
  pix = p;
  QLabel::setPixmap(scaledPixmap());
}

int Video::heightForWidth(int width) const
{
  return pix.isNull() ? this->height() : ((qreal)pix.height()*width)/pix.width();
}

int Video::widthForHeight(int height) const
{
  return pix.isNull() ? this->width() : ((qreal)pix.width()*height)/pix.height();
}

QSize Video::sizeHint() const
{
    int parentWidth = parent->width();
    int parentHeight = parent->height();

    int newWidth = widthForHeight(parentHeight);
    int newHeight = heightForWidth(parentWidth);

    if (newWidth <= parentWidth) {
        return QSize(newWidth, parentHeight);
    }

    if (newHeight <= parentHeight) {
        return QSize(parentWidth, newHeight);
    }

    return QSize(parentWidth, parentHeight);
}

QPixmap Video::scaledPixmap() const
{
    if (!pix.isNull()) {
        return pix.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    } else {
        return pix;
    }
}

void Video::resizeEvent(QResizeEvent *)
{
  setScaledContents(false);
  if(!pix.isNull())
      QLabel::setPixmap(scaledPixmap());
}
