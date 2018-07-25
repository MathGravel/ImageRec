#include "configuration.h"
#include <QDebug>

Configuration::Configuration()
{
    ifstream fichier("config.txt", ios::in);

    if (fichier) {
         qDebug() << "yes";
         /*
            string contenu;  // déclaration d'une chaîne qui contiendra la ligne lue
            getline(fichier, contenu);  // on met dans "contenu" la ligne
            cout << contenu;  // on affiche la ligne

            fichier.close();*/
    }
    else {
            //cerr << "Impossible d'ouvrir le fichier !" << endl;
        qDebug() << "no";
    }
}
