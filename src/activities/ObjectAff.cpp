/**
* \file	ObjectAff.cpp
* \author	Mathieu Gravel
* \version
* \date
* \brief
* \details
**/

#include "ObjectAff.h"

/**
* \fn	ObjectAffordances::ObjectAffordances(int numberClasses)
* \brief	Constructor of ObjectAffordances
* \detail	
* \param	numberClasses	int : Number of classes
**/
ObjectAffordances::ObjectAffordances(int numberClasses) {
    numClasses = numberClasses;
    frameCount = 0;
    nbClasses = 0;
    currentAff = false;
}


/**
* \fn	ObjectAffordances::clearCurrentAffordances()
* \brief	Clears the current affordances
**/
void ObjectAffordances::clearCurrentAffordances() {
    affordances.clear();
    frameCount = 0;
}


/**
* \fn	std::vector<AffordanceTime*> ObjectAffordances::findAffordances(DetectedObjects &regions, DetectedObjects &hands)
* \brief	Sets the affordances of the different Objects 
* \detail
* \param	&regions	DetectedObjects : All the objects detected except the hands
* \param	&hands	DetectedObjects : Hands detected on the screen	
**/
std::vector<AffordanceTime*> ObjectAffordances::findAffordances(DetectedObjects &regions, DetectedObjects &hands) {
    currentAff = false;
    frameCount++;
    std::vector<AffordanceTime*> currentAffordance;
    for (const auto obj : regions) { //Loop on the different objects
        for (auto & hand : hands) { //Loop on the hands
            AffordanceCheck regionsOverlap(obj,hand); //Defined in the .h file
            if (regionsOverlap) {
                currentAff = true;
                if (affordances.count(regionsOverlap.getObjectType()) > 0) //affordances : td::unordered_map<std::string,AffordanceTime> 
                    affordances.at(regionsOverlap.getObjectType()).markCurrentInteractions(obj.getDist(),obj.getObjPos(),obj.getProb(),frameCount);
                else
                    affordances[regionsOverlap.getObjectType()] = AffordanceTime(Affordance(obj.getObjName(),obj.getDist(),obj.getObjPos(),obj.getProb()),frameCount);

                affordances.at(regionsOverlap.getObjectType()).clean();
                if (affordances.at(regionsOverlap.getObjectType()).getNumberOfOccurences() > 9) {
                    affordances[regionsOverlap.getObjectType()].reset();
                    currentAffordance.push_back(&affordances[regionsOverlap.getObjectType()]);
                }
            }
        }
    }
    //Ajoute un check que si c plus que 5 secondes tu marque l'interaction comme finit.
    return currentAffordance;
}

/**
* \fn	ObjectAffordances::cleanMatrix()
* \brief	Cleans the Matrix confMatrix
* \detail
**/
void ObjectAffordances::cleanMatrix(){
    for (auto it : this->confMatrix){
        this->confMatrix[it.first].reset();
    }
}

ObjectAffordances::~ObjectAffordances() {}

std::istream &operator>>(std::istream &is, ObjectAffordances &objaff) {
    return is;
}


