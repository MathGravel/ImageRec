#ifndef AFFORDANCE_H
#define AFFORDANCE_H

#include <opencv2/opencv.hpp>
#include <ctime>
#include <sstream>
#include <list>


class Affordance {

public:
    Affordance(std::string obj = "teacup", double prob=76.7653);
    Affordance(std::string obj,double pos,cv::Rect reg,double prob);


    std::string getName() const {return objectName;}
    double getObjectPosition() const {return objectPos;}
    const cv::Rect& getRegion() const {return region;}
    double getObjectProbability() const {return objProb;}
    void setDist(double dist) {
        objectPos = dist;
    }

    bool operator ==(const Affordance& autre);
    bool operator <(const Affordance& autre);


    std::string to_str() const {
        std::stringstream ss;
        ss << this->objectName << " " << this->objProb;
        return ss.str();
    }


private:

    std::string objectName;
    double objectPos;
    cv::Rect region;
    double objProb;
    //Date
    //To_String Format (obj)_(x.xx)
    //Si t'a aucune interaction envoie un objet void i.e un appel constructeur vide

    friend std::ostream& operator <<(std::ostream& o, const Affordance a) {
        return o << a.objectName << " " << a.objProb;
    }



};


class AffordanceTime {

public:

    AffordanceTime();

    AffordanceTime(Affordance aff);


    void markCurrentInteractions(double dist);

    void markCurrentInteractions(double dist, cv::Rect pos,double prob);

    double getInteractionTime() {
        return times.front() - times.back();
    }

    double getStartTime() {
        return times.front();
    }


    Affordance& getAffordance() {
        return affordance;
    }

    std::string getName() const {
        return affordance.getName();
    }

    int getNumberOfOccurences() {
        return times.size();
    }

    void clean();
    void reset() {times.clear();}


    bool checkAffName(const std::string className) const {
        return affordance.getName() == className;
    }

    bool operator ==(const AffordanceTime& autre);
    bool operator <(const AffordanceTime& autre) ;


private:

    Affordance affordance;
    double startTime;
    double currentTime;
    std::list<double> times;




    friend std::ostream& operator <<(std::ostream& o, const AffordanceTime a) {
        return o << a.affordance;
    }

};


#endif //AFFORDANCE_H
