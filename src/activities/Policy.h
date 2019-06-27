#ifndef POLICY_H
#define POLICY_H

#include <string>
#include "Affordance.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <QString>
#include <QFile>
#include <QTextStream>
#include "PARC/include/extendedPlanLibrary.h"
#include "PARC/include/planLibrary.h"
#include "PARC/include/solver.h"

using namespace std;

class Policy {

public:
    Policy();
    bool update(Affordance* observation);

    std::vector<std::pair<std::string,float>> getNextActions();
    std::vector<std::pair<std::string,float>> getGoalsProba();

private:

    bool load();
    extendedPlanLibrary ePL;
    solver gP;

};


#endif //POLICY_H
