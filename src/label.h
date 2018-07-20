#ifndef LABEL_H
#define LABEL_H

#include <QWidget>
#include <QLabel>
#include <QResizeEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QFont>
#include <QSize>

class Label : public QLabel
{
    Q_OBJECT

    public:
        explicit Label(QWidget *parent = nullptr);

    public slots:
        void resizeEvent(QResizeEvent *);

    private:
        int fontSize;
};

#endif // LABEL_H
