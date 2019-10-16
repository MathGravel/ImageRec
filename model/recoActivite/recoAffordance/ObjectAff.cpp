#include "ObjectAff.h"
const String NULLNAME = "NULL";

ObjectAffordances::ObjectAffordances(int numberClasses) {
    numClasses = numberClasses; //what is classes
    frameCount = 0;
    nbClasses = 0; //what is classes
    //currentAff = false;
}



void ObjectAffordances::clearCurrentAffordances() {
    affordances.clear();
    frameCount = 0;
}

std::vector<AffordanceTime*> ObjectAffordances::findAffordances(DetectedObjects &regions,
                                                                DetectedObjects &hands,
                                                                MatriceAffordance &objectMat,
                                                                bool removeLastValue) {
    double VAL = 100;
    
    //currentAff = false;
    frameCount++;
    std::vector<AffordanceTime*> currentAffordances;//vector of AffordanceTime in which the times of affordance will be stored.
    for (const auto & obj : regions) {
        for (auto & hand : hands) {

            //double objFreq = objectMat.getFrequence(obj.getObjName());
            //double dist = sqrt( pow( ( (obj.getObjPos().y+obj.getObjPos().height/2) - (hand.getObjPos().y+hand.getObjPos().height/2) ),2) + pow( ( (obj.getObjPos().x+obj.getObjPos().width/2) - (hand.getObjPos().x+hand.getObjPos().width/2) ),2));

            if((obj.getObjPos() & hand.getObjPos()).area() > (0.3 * std::min(obj.getObjPos().area(),hand.getObjPos().area()))
             && abs(obj.getDist()-hand.getDist())<0.05 )
            {
		
               //double prob=objectMat.getFrequence(obj.getObjName());
               double prob=objectMat.probCalculation(obj.getObjName(),obj.getProb());
               Affordance* aff = new Affordance(obj.getObjName(),0,obj.getObjPos(),prob,obj.getDist());
               AffordanceTime* currentAffordance = new AffordanceTime(*aff,frameCount);
               currentAffordances.push_back(currentAffordance);
            }

            }
        }
    if(currentAffordances.empty())
    {
		//double nullFreq = objectMat.getFrequence(NULLNAME);
        double nullFreq = objectMat.probCalculation(NULLNAME,0.45);
        Affordance* aff = new Affordance(NULLNAME,0,cv::Rect(),nullFreq,0);
        AffordanceTime* currentAffordance = new AffordanceTime(*aff,frameCount);
        //std::cout<<"Il y a une affordance avec : "<<aff->getName()<< " avec une proba : "<<aff->getObjectProbability() <<std::endl;
        currentAffordances.push_back(currentAffordance);
    }

    objectMat.update(currentAffordances, removeLastValue);

    return currentAffordances;
}

std::vector<AffordanceTime*> ObjectAffordances::addNull(MatriceAffordance &objectMat, bool removeLastValue){
        std::vector<AffordanceTime*> currentAffordances;
        double nullFreq = objectMat.probCalculation(NULLNAME,0.40); 
        Affordance* aff = new Affordance(NULLNAME,0,cv::Rect(),nullFreq,0);
        AffordanceTime* currentAffordance = new AffordanceTime(*aff,frameCount);
        currentAffordances.push_back(currentAffordance);
    	objectMat.update(currentAffordances, removeLastValue);
return currentAffordances;
}

ObjectAffordances::~ObjectAffordances() {}


std::istream &operator>>(std::istream &is, ObjectAffordances &objaff) {
    return is;
}


