#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class Configuration
{
    public:
        Configuration();

    private:
        string sourceType;
        string sourceChemin;
        string affichageActions;
        string affichageObjets;
        string affichageMains;
        int precisionTraitement;
        string langue;
};

#endif // CONFIGURATION_H
