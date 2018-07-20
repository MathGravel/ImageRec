#ifndef FENETREPARAMETRES_H
#define FENETREPARAMETRES_H

#include <QDialog>

namespace Ui {
    class FenetreParametres;
}

class FenetreParametres : public QDialog
{
    Q_OBJECT

    public:
        explicit FenetreParametres(QWidget *parent = 0);
        ~FenetreParametres();

    private:
        Ui::FenetreParametres *ui;
};

#endif // FENETREPARAMETRES_H
