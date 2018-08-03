#ifndef IMAGE_H
#define IMAGE_H

#include <QLabel>
#include <QPixmap>
#include <QResizeEvent>

class Image : public QLabel
{
    Q_OBJECT

    public:
        explicit Image(QWidget *parent = 0);
        virtual int heightForWidth( int width ) const;
        virtual QSize sizeHint() const;
        QPixmap scaledPixmap() const;

    public slots:
        void setPixmap ( const QPixmap & );
        void resizeEvent(QResizeEvent *);

    private:
        QPixmap pix;
};

#endif // IMAGE_H
