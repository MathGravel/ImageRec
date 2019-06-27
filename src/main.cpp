#include "interface/configuration.h"
#include "interface/fenetreprincipale.h"
#include <QApplication>
#include <QTranslator>


// Variables globales
string fichierConfiguration = "settings.ini";
Configuration parametres;

int main(int argc, char *argv[])
{
    // Création de l'application Qt
    QApplication app(argc, argv);


    FenetrePrincipale fenetre;
    fenetre.configuration();
    fenetre.show();


    return app.exec();

}
