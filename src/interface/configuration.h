#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
#include <map>

using namespace std;

extern string fichierConfiguration;

class Configuration
{
    public:
        Configuration();
        void chargementParametres();
        void sauvegardeParametres();
        string getParametre(string nom);
        void setParametre(string nom, string valeur);
        map<string,string> getParametres() {return parametres;}

    private:
        map <string, string> parametres;
};

#endif // CONFIGURATION_H
