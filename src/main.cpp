#include "configuration.h"
#include "fenetreprincipale.h"
#include <QApplication>

// Variable globale
Configuration parametres;

int main(int argc, char *argv[])
{
    // Création de l'application Qt
    QApplication app(argc, argv);

    // Création et affichage de la fenêtre principale
    FenetrePrincipale fenetre;
    fenetre.configuration(&app);
    fenetre.show();

    return app.exec();
}
