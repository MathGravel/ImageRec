#include "ObjectAff.h"

ObjectAffordances::ObjectAffordances(int numberClasses) {
    numClasses = numberClasses;
    nbClasses = 0;
    confMatrix = new double*[numClasses];
    for (int i  = 0; i < numClasses;i++)
        confMatrix[i] = new double[numClasses];
}



void ObjectAffordances::clearCurrentAffordances() {
    affordances.clear();
}



std::vector<AffordanceTime*> ObjectAffordances::findAffordances(DetectedObjects &regions, DetectedObjects &hands) {


    std::vector<AffordanceTime*> currentAffordance;
    for (const auto obj : regions) {

        for (auto & hand : hands) {
            AffordanceCheck regionsOverlap(obj,hand);
            if (regionsOverlap) {
                if (affordances.count(regionsOverlap.getObjectType()) > 0)
                    affordances.at(regionsOverlap.getObjectType()).markCurrentInteractions(obj.getDist(),obj.getObjPos(),obj.getProb());
                else
                    affordances[regionsOverlap.getObjectType()] = AffordanceTime(Affordance(obj.getObjName(),obj.getDist(),obj.getObjPos(),obj.getProb()));

                currentAffordance.push_back(&affordances[regionsOverlap.getObjectType()]);

            }}
    }

    //Ajoute un check que si c plus que 5 secondes tu marque l'interaction comme finit.
    return currentAffordance;
}


std::vector<AffordanceTime*> ObjectAffordances::findAffordances(std::vector<DetectedObject> &regions, DetectedObjects &hands) {


    std::vector<AffordanceTime*> currentAffordance;
    for (DetectedObject& obj : regions) {
        for (auto & hand : hands) {

            AffordanceCheck regionsOverlap(obj,hand);
            if (regionsOverlap) {
                if (affordances.count(regionsOverlap.getObjectType()) > 0)
                    affordances.at(regionsOverlap.getObjectType()).markCurrentInteractions(obj.getDist(),obj.getObjPos(),obj.getProb());
                else
                    affordances[regionsOverlap.getObjectType()] = AffordanceTime(Affordance(obj.getObjName(),obj.getDist(),obj.getObjPos(),obj.getProb()));

                currentAffordance.push_back(&affordances[regionsOverlap.getObjectType()]);

            }}
    }

    //Ajoute un check que si c plus que 5 secondes tu marque l'interaction comme finit.
    return currentAffordance;
}


//Fonction a completer.
bool  ObjectAffordances::checkifNewInteraction(DetectedObject & obj) {

    return true;
}

ObjectAffordances::~ObjectAffordances() {

    for (int i  = 0; i < numClasses;i++)
        delete [] confMatrix[i];
    delete[] confMatrix;

}

void ObjectAffordances::setConfusionMatrix(int noClasses, double** matrix) {

    numClasses = noClasses;

    for (int i  = 0; i < numClasses;i++)
        delete [] confMatrix[i];
    delete[] confMatrix;

    confMatrix = matrix;

}


std::istream &operator>>(std::istream &is, ObjectAffordances &objaff) {

    int noClasses = 0;
    double** matrix;
    if (is) {
        is >> noClasses;
    }

    matrix = new double*[noClasses];
    for (int i  = 0; i < noClasses;i++)
        matrix[i] = new double[noClasses];

    int col = -1;
    for (int i  = 0; i < pow(noClasses,2); i++) {
        if ((i % noClasses) == 0) {
            col++;
        }
        is >> matrix[col][i % noClasses];
    }

    objaff.setConfusionMatrix(noClasses,matrix);


    return is;
}


