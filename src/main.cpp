#include "configuration.h"
#include "fenetreprincipale.h"
#include <QApplication>

// Variables globales
string fichierConfiguration = "settings/config.ini";
Configuration parametres;

int main(int argc, char *argv[])
{
    // Création de l'application Qt
    QApplication app(argc, argv);

    // Chargement langue

    // Création et affichage de la fenêtre principale
    FenetrePrincipale fenetre;
    fenetre.configuration();
    fenetre.show();

    return app.exec();
}
