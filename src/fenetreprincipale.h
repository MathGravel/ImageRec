#ifndef FENETREPRINCIPALE_H
#define FENETREPRINCIPALE_H

#include <QMainWindow>
#include <QApplication>
#include <QStyle>
#include <QDesktopWidget>
#include <QFontDatabase>
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
        void configuration(QApplication *app);

    public slots:
        void gestionVideo();
        void ouvrirFenetreParametres();
        void pleinEcranVideo();

    private:
        Ui::FenetrePrincipale *ui;
        FenetreParametres *fenetreParametres;
};

#endif // FENETREPRINCIPALE_H
