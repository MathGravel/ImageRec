#ifndef VIDEO_H
#define VIDEO_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QResizeEvent>

class Video : public QLabel
{
    Q_OBJECT

    public:
        explicit Video(QWidget *parent = 0);
        virtual int heightForWidth(int width) const;
        virtual int widthForHeight(int height) const;
        virtual QSize sizeHint() const;
        QPixmap scaledPixmap() const;

    public slots:
        void setPixmap(const QPixmap &);
        void resizeEvent(QResizeEvent *);

    private:
        QWidget *parent;
        QPixmap pix;
};

#endif // VIDEO_H
