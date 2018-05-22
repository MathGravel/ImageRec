//
// Created by uqamportable on 16/04/18.
//

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



AffordanceTime* ObjectAffordances::findAffordances(DetectedObjects &regions, DetectedObject &hand) {


    AffordanceTime * currentAffordance = NULL;
    for (DetectedObject& obj : regions) {

        AffordanceCheck regionsOverlap(obj,hand);
        if (regionsOverlap) {
            if (affordances.count(regionsOverlap.getObjectType()) > 0)
                affordances.at(regionsOverlap.getObjectType()).markCurrentInteractions(obj.getDist(),obj.getObjPos(),obj.getProb());
            else
                affordances[regionsOverlap.getObjectType()] = AffordanceTime(Affordance(obj.getObjName(),obj.getDist(),obj.getObjPos(),obj.getProb()));

            currentAffordance = &affordances[regionsOverlap.getObjectType()];
            std::cout << *currentAffordance << std::endl;

        }
    }

    //Ajoute un check que si c plus que 5 secondes tu marque l'interaction comme finit.
    return currentAffordance;
}


AffordanceTime* ObjectAffordances::findAffordance(std::vector<DetectedObject> &regions, DetectedObject &hand) {


    AffordanceTime * currentAffordance = NULL;
    for (DetectedObject& obj : regions) {

        AffordanceCheck regionsOverlap(obj,hand);
        if (regionsOverlap) {
            if (affordances.count(regionsOverlap.getObjectType()) > 0)
                affordances.at(regionsOverlap.getObjectType()).markCurrentInteractions(obj.getDist(),obj.getObjPos(),obj.getProb());
            else
                affordances[regionsOverlap.getObjectType()] = AffordanceTime(Affordance(obj.getObjName(),obj.getDist(),obj.getObjPos(),obj.getProb()));

            currentAffordance = &affordances.at(regionsOverlap.getObjectType());
            std::cout << *currentAffordance << std::endl;

        }
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

    int noClasses;
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


