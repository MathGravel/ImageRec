//
// Created by uqamportable on 16/04/18.
//

#ifndef IMAGEREC_AFFORDANCE_H
#define IMAGEREC_AFFORDANCE_H

#include <opencv2/opencv.hpp>
#include <ctime>



class Affordance {

public:
    Affordance() {}
    Affordance(std::string obj,double pos,cv::Rect reg,double prob) :
            objectName(obj),objectPos(pos),region(reg),objProb(prob){}


    std::string getName() const {return objectName;};
    double getObjectPosition() const {return objectPos;};
    const cv::Rect& getRegion() const {return region;};
    double getObjectProbability() const {return objProb;};


private:

    std::string objectName;
    double objectPos;
    cv::Rect region;
    double objProb;
    //Date
    //To_String Format (obj)_(x.xx)
    //Si t'a aucune interaction envoie un objet void i.e un appel constructeur vide

};


class AffordanceTime {

public:

    AffordanceTime(){
        startTime = std::clock() / (double) CLOCKS_PER_SEC;
    }

    AffordanceTime(Affordance aff) {
        startTime = std::clock() / (double) CLOCKS_PER_SEC;
        affordance = aff;
    }



    void markCurrentInteractions() {
        currentTime  = std::clock() / (double) CLOCKS_PER_SEC;
    }

    double getInteractionTime() {
        return startTime - currentTime;
    }


    Affordance& getAffordance() {
        return affordance;
    }

    bool checkAffName(std::string className) {
        return affordance.getName() == className;
    }


private:

    Affordance affordance;
    double startTime;
    double currentTime;


};


#endif //IMAGEREC_AFFORDANCE_H
