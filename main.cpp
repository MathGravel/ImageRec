//
// Created by uqamportable on 02/08/18.
//
#include "RecoManager.h"
#include <map>

using namespace std;
int main(int argc, char** argv)
{
    map <string, string> parametres;
    parametres["sourceType"] = "Realsense";
    parametres["sourceChemin"] = "/home/uqamportable/Videos/vicColor.mp4";
    parametres["sourceCheminDepth"] = "/home/uqamportable/Videos/vidDepth.mp4";
    parametres["affichageActions"] = "Zone, nom et pourcentage";
    parametres["affichageObjets"] = "Zone";
    parametres["affichageMains"] = "Zone et poucentage";
    parametres["precisionTraitement"] = "75 %";
    parametres["Show Name"] = "1";
    parametres["Show Zone"] = "1";
    parametres["Show Percentage"] = "1";
    parametres["langue"] = "Français";
    RecoManager rec(parametres);
    rec.update();

    return 0;
}