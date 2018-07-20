#ifndef BUTTON_H
#define BUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QResizeEvent>
#include <QFont>
#include <QSize>

class Button : public QPushButton
{
    Q_OBJECT

    public:
        explicit Button(QWidget *parent = nullptr);

    public slots:
        void resizeEvent(QResizeEvent *);
};

#endif // BUTTON_H
