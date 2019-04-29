#include "ObjectAff.h"

ObjectAffordances::ObjectAffordances(int numberClasses) {
    numClasses = numberClasses;
    frameCount = 0;
    nbClasses = 0;
    currentAff = false;
}



void ObjectAffordances::clearCurrentAffordances() {
    affordances.clear();
    frameCount = 0;
}



std::vector<AffordanceTime*> ObjectAffordances::findAffordances(DetectedObjects &regions, DetectedObjects &hands) {

    currentAff = false;
    frameCount++;
    std::vector<AffordanceTime*> currentAffordance;
    for (const auto obj : regions) {

        for (auto & hand : hands) {
            AffordanceCheck regionsOverlap(obj,hand);
            if (regionsOverlap) {
                currentAff = true;
                if (affordances.count(regionsOverlap.getObjectType()) > 0)
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

void ObjectAffordances::cleanMatrix(){
    for (auto it : this->confMatrix){
        this->confMatrix[it.first].reset();
    }
}

ObjectAffordances::~ObjectAffordances() {}


std::istream &operator>>(std::istream &is, ObjectAffordances &objaff) {
    return is;
}


