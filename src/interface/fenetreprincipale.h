#ifndef FENETREPRINCIPALE_H
#define FENETREPRINCIPALE_H

#include "fenetreparametres.h"
#include <QMainWindow>
#include <QApplication>
#include <QStyle>
#include <QDesktopWidget>
//#include "../activities/RecoManager.h"

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
        //RecoManager *rec;
};

#endif // FENETREPRINCIPALE_H
