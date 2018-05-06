//
// Created by uqamportable on 16/04/18.
//

#ifndef IMAGEREC_AFFORDANCE_H
#define IMAGEREC_AFFORDANCE_H

#include <opencv2/opencv.hpp>
#include <ctime>
#include <sstream>


class Affordance {

public:
    Affordance(std::string obj = "teacup", double prob=76.7653) {
        objectName = obj;
        objProb = prob;
    }
    Affordance(std::string obj,double pos,cv::Rect reg,double prob) :
            objectName(obj),objectPos(pos),region(reg),objProb(prob){}


    std::string getName() const {return objectName;};
    double getObjectPosition() const {return objectPos;};
    const cv::Rect& getRegion() const {return region;};
    double getObjectProbability() const {return objProb;};

    std::string to_str() const {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    };


private:

    std::string objectName;
    double objectPos;
    cv::Rect region;
    double objProb;
    //Date
    //To_String Format (obj)_(x.xx)
    //Si t'a aucune interaction envoie un objet void i.e un appel constructeur vide

    friend std::ostream& operator <<(std::ostream& o, const Affordance a) {
        return o << "(" << a.objectName << ")(" << std::setprecision(4) << a.objProb << ")" << std::endl;
    }



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


    friend std::ostream& operator <<(std::ostream& o, const AffordanceTime a) {
        return o << a.affordance;
    }

};


#endif //IMAGEREC_AFFORDANCE_H
