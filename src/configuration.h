#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

extern string fichierConfiguration;

class Configuration
{
    public:
        Configuration();
        void chargementParametres();
        void sauvegardeParametres();
        void creationFichier(bool ecraser=false);
        string getSourceType() const;
        void setSourceType(const string &value);
        string getSourceChemin() const;
        void setSourceChemin(const string &value);
        string getAffichageActions() const;
        void setAffichageActions(const string &value);
        string getAffichageObjets() const;
        void setAffichageObjets(const string &value);
        string getAffichageMains() const;
        void setAffichageMains(const string &value);
        string getPrecisionTraitement() const;
        void setPrecisionTraitement(string value);
        string getLangue() const;
        void setLangue(const string &value);

private:
        string sourceType = "Vidéo locale";
        string sourceChemin = "/home/baptiste/Vidéos/demo.mp4";
        string affichageActions = "Zone, nom et pourcentage";
        string affichageObjets = "Zone";
        string affichageMains = "Zone et poucentage";
        string precisionTraitement = "75 %";
        string langue = "Français";
};

#endif // CONFIGURATION_H
