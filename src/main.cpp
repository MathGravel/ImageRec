#include "interface/configuration.h"
#include "interface/fenetreprincipale.h"
#include <QApplication>
#include <QTranslator>
#include "plans/include/Domain.h"


// Variables globales
string fichierConfiguration = "settings.ini";
Configuration parametres;

int main(int argc, char *argv[])
{
    // Création de l'application Qt
    QApplication app(argc, argv);

    Domain domain = SmallDomain::getSmallDomain();

    /*Solver s = Solver(domain,1,500);
    s.addObservation("hold(teakettle)");
    s.addObservation("hold(water)");

    s.addObservation("hold(mug)");
    auto tt = s.getNextActions();
    std::cout << "Check " << tt.begin()->first << " " << tt.begin()->second << std::endl;
    std::cout << "Check " << (++(tt.begin()))->first << " " << (++(tt.begin()))->second << std::endl;
    s.addObservation("hold(choco)");

    tt = s.getGoalsProba();
    std::cout << "Check " << tt.begin()->first << " " << tt.begin()->second << std::endl;
*/
    // Création et affichage de la fenêtre principale
    FenetrePrincipale fenetre;
    fenetre.configuration();
    fenetre.show();


    return app.exec();
    /*Domain domain = SmallDomain::getSmallDomain();

    Solver s = Solver(domain,1,500);
    s.addObservation("hold(water)");
    auto tt = s.getGoalsProba();
    s.addObservation("hold(teakettle)");
     tt = s.getGoalsProba();

    s.addObservation("hold(mug)");
     tt = s.getGoalsProba();
     auto check = s.getNextActions();
     std::cout << "Goals " << tt.begin()->first<< " Actions " << check.begin()->first << std::endl;

    s.addObservation("hold(tea)");
     tt = s.getGoalsProba();
     check = s.getNextActions();
    std::cout << "Goals " << tt.size() << " Actions " << check.size() << std::endl;


    return 1;*/
}
