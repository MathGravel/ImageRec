#include "image.h"

Image::Image(QWidget *parent) : QLabel(parent)
{
  this->setMinimumSize(1,1);
  setScaledContents(false);
}

void Image::setPixmap(const QPixmap &p)
{
  pix = p;
  QLabel::setPixmap(scaledPixmap());
}

int Image::heightForWidth(int width) const
{
  return pix.isNull() ? this->height() : ((qreal)pix.height()*width)/pix.width();
}

QSize Image::sizeHint() const
{
  int w = this->width();
  return QSize( w, heightForWidth(w) );
}

QPixmap Image::scaledPixmap() const
{
  return pix.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void Image::resizeEvent(QResizeEvent *)
{
  setScaledContents(false);
  if(!pix.isNull())
      QLabel::setPixmap(scaledPixmap());
}
