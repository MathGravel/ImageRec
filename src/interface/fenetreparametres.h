#ifndef FENETREPARAMETRES_H
#define FENETREPARAMETRES_H

#include "configuration.h"
#include <QDialog>
#include <QString>
#include <QFileDialog>
#include <QCloseEvent>

extern Configuration parametres;

namespace Ui {
    class FenetreParametres;
}

class FenetreParametres : public QDialog
{
    Q_OBJECT

    public:
        explicit FenetreParametres(QWidget *parent = 0);
        ~FenetreParametres();
        void chargementParametres();
        void sauvegardeParametres();
        void selectionVideoCouleur();
        void selectionVideoProfondeur();

    public slots:
        void fermer();
        void enregistrer();
        void sourceChoix(QString valeur);
        void messageCouleur();
        void messageProfondeur();
        void actionsChoix(QString valeur);
        void objetsChoix(QString valeur);
        void mainsChoix(QString valeur);
        void traitementChoix(QString valeur);
        void langueChoix(QString valeur);

    private:
        Ui::FenetreParametres *ui;
        map <QString, QString> parametresTemporaires;
};

#endif // FENETREPARAMETRES_H
