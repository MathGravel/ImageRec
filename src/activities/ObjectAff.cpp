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


/*
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
}*/
/**
std::vector<AffordanceTime*> ObjectAffordances::findAffordances(DetectedObjects &regions,
                                                                DetectedObjects &hands) {
    **/
std::vector<AffordanceTime*> ObjectAffordances::findAffordances(DetectedObjects &regions,
                                                                DetectedObjects &hands,
                                                                ObjectsMat &objectMat,
                                                                bool supAtime) {

    double VAL = 100;
    String NULLNAME = "NULL";
    currentAff = false;
    frameCount++;
    std::vector<AffordanceTime*> currentAffordances;///azerty2
    for (const auto obj : regions) {
        for (auto & hand : hands) {

            double objFreq = objectMat.getFrequence(obj.getObjName());
            double dist = sqrt( pow( ( (obj.getObjPos().y+obj.getObjPos().height/2) - (hand.getObjPos().y+hand.getObjPos().height/2) ),2) +
                             pow( ( (obj.getObjPos().x+obj.getObjPos().width/2) - (hand.getObjPos().x+hand.getObjPos().width/2) ),2));

            if((obj.getObjPos() & hand.getObjPos()).area() > 0 || dist<VAL)
            {
                double freqObj=objectMat.getFrequence(obj.getObjName());
                double prob=freqObj*0.8+((VAL-dist)/VAL)*0.2;
                //std::cout << freqObj<< std::endl;
                //std::cout << (VAL-dist)/VAL<< std::endl;
                //std::cout<<"Il y a une affordance  avec : "<<obj.getObjName()<< " avec une proba : "<<prob<<std::endl;
               Affordance* aff = new Affordance(obj.getObjName(),0,obj.getObjPos(),prob,dist);
               AffordanceTime* currentAffordance = new AffordanceTime(*aff,frameCount);
               currentAffordances.push_back(currentAffordance);
            }

            }
        }
    if(currentAffordances.empty())
    {
        double nullFreq = objectMat.getFrequence(NULLNAME);
        Affordance* aff = new Affordance(NULLNAME,0,cv::Rect(),nullFreq,0);
        AffordanceTime* currentAffordance = new AffordanceTime(*aff,frameCount);
        std::cout<<"Il y a une affordance avec : "<<aff->getName()<< " avec une proba : "<<aff->getObjectProbability() <<std::endl;
        currentAffordances.push_back(currentAffordance);
    }

    objectMat.update(currentAffordances, supAtime);

    return currentAffordances;
}

ObjectAffordances::~ObjectAffordances() {}


std::istream &operator>>(std::istream &is, ObjectAffordances &objaff) {
    return is;
}


