#ifndef FENETREPRINCIPALE_H
#define FENETREPRINCIPALE_H

#include <QMainWindow>
#include "fenetreparametres.h"

namespace Ui {
    class FenetrePrincipale;
}

class FenetrePrincipale : public QMainWindow
{
    Q_OBJECT

    public:
        explicit FenetrePrincipale(QWidget *parent = 0);
        ~FenetrePrincipale();

    public slots:
        void ouvrirFenetreParametres();

    private:
        Ui::FenetrePrincipale *ui;
        FenetreParametres *fenetreParametres;
};

#endif // FENETREPRINCIPALE_H
