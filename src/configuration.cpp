#include "configuration.h"

Configuration::Configuration()
{
    chargementParametres();
}

void Configuration::chargementParametres()
{
    ifstream fichier(fichierConfiguration, ios::in);

    if (fichier) {
        string ligne;

        while(getline(fichier, ligne)) {
            //cout << ligne << endl;
        }

        fichier.close();
    }
    else {
        cout << "Impossible d'ouvrir le fichier de configuration !" << endl;
    }
}

void Configuration::sauvegardeParametres()
{
    cout << "Sauvegarde..." << endl;
}

void Configuration::creationFichier(bool ecraser)
{
    if (ecraser) {
        // ...
    }
}

string Configuration::getSourceType() const
{
    return sourceType;
}

void Configuration::setSourceType(const string &value)
{
    sourceType = value;
}

string Configuration::getSourceChemin() const
{
    return sourceChemin;
}

void Configuration::setSourceChemin(const string &value)
{
    sourceChemin = value;
}

string Configuration::getAffichageActions() const
{
    return affichageActions;
}

void Configuration::setAffichageActions(const string &value)
{
    affichageActions = value;
}

string Configuration::getAffichageObjets() const
{
    return affichageObjets;
}

void Configuration::setAffichageObjets(const string &value)
{
    affichageObjets = value;
}

string Configuration::getAffichageMains() const
{
    return affichageMains;
}

void Configuration::setAffichageMains(const string &value)
{
    affichageMains = value;
}

string Configuration::getPrecisionTraitement() const
{
    return precisionTraitement;
}

void Configuration::setPrecisionTraitement(string value)
{
    precisionTraitement = value;
}

string Configuration::getLangue() const
{
    return langue;
}

void Configuration::setLangue(const string &value)
{
    langue = value;
}
