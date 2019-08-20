/**
* \file Policy.h
* \author Mathieu Gravel
* \version
* \date
* \brief
* \details
**/

#ifndef POLICY_H
#define POLICY_H

#include <string>
#include "../recoActivite/recoAffordance/Affordance.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include "PARC/include/extendedPlanLibrary.h"
#include "PARC/include/planLibrary.h"
#include "PARC/include/solver.h"

using namespace std;

class Policy {

public:
    /**
    * \fn	Policy()
    * \brief	Constructor of Policy
    **/
    Policy();

    /**
     * \fn update(Affordance* observation)
     * \brief adds the current observation in gP
     * \param observation : Affordance* observation
     */
    bool update(Affordance* observation);

    /**
     * \fn Reset()
     * \brief gP becomes a new solver
     */
    void Reset();

    /**
     * \brief getNextActions
     * \return
     */
    std::vector<std::pair<std::string,float>> getNextActions();

    /**
     * \brief getGoalsProba
     * \return
     */
    std::vector<std::pair<std::string,float>> getGoalsProba();

//Was private, maybe for a good reason | As been moved
/**
 * \fn load(string pathDomain)
 * \param pathDomain : string => path to choose Domain
 * \return boolean
*/
    bool load(string pathDomain);
private:

    extendedPlanLibrary ePL;
    solver gP;

};


#endif //POLICY_H
