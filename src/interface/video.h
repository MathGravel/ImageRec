#ifndef VIDEO_H
#define VIDEO_H

#include <QWidget>
#include <QFrame>
#include <QResizeEvent>

class Video : public QFrame
{
    Q_OBJECT

    public:
        explicit Video(QWidget *parent = nullptr);

    public slots:
        void resizeEvent(QResizeEvent *);
};

#endif // VIDEO_H
