#include "configuration.h"
#include <unistd.h>
#include <limits.h>
Configuration::Configuration()
{
    // Paramètres par défaut
    parametres["sourceType"] = "Vidéo locale";
    parametres["sourceChemin"] = "/home/baptiste/Vidéos/";
    parametres["affichageActions"] = "Zone, nom et pourcentage";
    parametres["affichageObjets"] = "Zone";
    parametres["affichageMains"] = "Zone et poucentage";
    parametres["precisionTraitement"] = "75 %";
    parametres["langue"] = "Français";

    // Chargement des paramètres du fichier
    chargementParametres();
}

void Configuration::chargementParametres()
{
    ifstream fichier(fichierConfiguration, ios::in);

    if (fichier) {
        string ligne;

        while(getline(fichier, ligne)) {

            string nom = ligne.substr(0, ligne.find("="));
            string valeur = ligne.substr(ligne.find("=")+1);

            parametres[nom] = valeur;
        }
        fichier.close();
    }
    else {
        cout << "Impossible d'ouvrir le fichier de configuration !" << endl;
    }
}

void Configuration::sauvegardeParametres()
{
    ofstream fichier(fichierConfiguration, ios::out | ios::trunc);

    if (fichier) {

        for(map<string,string>::iterator it=parametres.begin() ; it!=parametres.end() ; ++it) {
            fichier << it->first << "=" << it->second << endl;
        }

        fichier.close();
    }
    else {
        cout << "Impossible d'ouvrir le fichier de configuration !" << endl;
    }
}

string Configuration::getParametre(string nom)
{
    return parametres[nom];
}

void Configuration::setParametre(string nom, string valeur)
{
    parametres[nom] = valeur;
}
