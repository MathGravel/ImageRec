#ifndef FENETREPRINCIPALE_H
#define FENETREPRINCIPALE_H

#include "fenetreparametres.h"
#include <QMainWindow>
#include <QApplication>
#include <QStyle>
#include <QDesktopWidget>

namespace Ui {
    class FenetrePrincipale;
}

class FenetrePrincipale : public QMainWindow
{
    Q_OBJECT

    public:
        explicit FenetrePrincipale(QWidget *parent = 0);
        ~FenetrePrincipale();
        void configuration();

    public slots:
        void gestionVideo();
        void MiseAJourImage();
        void MiseAJourHistogramme();
        void MiseAJourProgression();
        void MiseAJourInformations();
        void ouvrirFenetreParametres();
        void pleinEcranVideo();

    private:
        Ui::FenetrePrincipale *ui;
        FenetreParametres *fenetreParametres;
};

#endif // FENETREPRINCIPALE_H
