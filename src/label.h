#ifndef LABEL_H
#define LABEL_H

#include <QWidget>
#include <QLabel>

class Label : public QLabel
{
    Q_OBJECT

    public:
        explicit Label(QWidget *parent = nullptr);

    signals:
        void clicked();

    protected:
        void mousePressEvent(QMouseEvent*);
};

#endif // LABEL_H
