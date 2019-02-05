#include "Affordance.h"

Affordance::Affordance(std::string obj , double prob) {
    objectName = obj;
    objProb = prob;
    objectPos = 0;
    if (objectName == "teapot") {
        objectName = "teakettle";
    }
    if(objectName == "chocolate")
        objectName = "choco";
    if (objectName == "pitcher")
        objectName = "water";


}
Affordance::Affordance(std::string obj,double pos,cv::Rect reg,double prob) :
        objectName(obj),objectPos(pos),region(reg),objProb(prob){
    if (objectName == "teapot") {
        objectName = "teakettle";
    }
    if(objectName == "chocolate")
        objectName = "choco";
    if (objectName == "pitcher")
        objectName = "water";

}

bool Affordance::operator ==(const Affordance& autre) {
    if (this == &autre)
        return true;
    cv::Point p1(this->region.x + this->region.width/2,this->region.y + this->region.height/2);

    cv::Point p2(autre.region.x + autre.region.width/2,autre.region.y + autre.region.height/2);

    double dist = cv::norm(p1 - p2);
    return this->objectName == autre.objectName && dist < 100;
}
bool Affordance::operator <(const Affordance& autre) {
    if (this == &autre)
        return false;
    cv::Point p1(this->region.x + this->region.width/2,this->region.y + this->region.height/2);

    cv::Point p2(autre.region.x + autre.region.width/2,autre.region.y + autre.region.height/2);
    double dist = cv::norm(p1 - p2);
    return this->objectName < autre.objectName || dist > 100;
}

AffordanceTime::AffordanceTime(){
    startTime = std::clock() / (double) CLOCKS_PER_SEC;
    currentTime  = std::clock() / (double) CLOCKS_PER_SEC;
}

AffordanceTime::AffordanceTime(Affordance aff,int frameCount) {
    startTime = std::clock() / (double) CLOCKS_PER_SEC;
    affordance = aff;
    currentTime  = std::clock() / (double) CLOCKS_PER_SEC;
    times.push_back(frameCount);

}


void AffordanceTime::markCurrentInteractions(double dist,int frameCount) {
    this->affordance.setDist(dist);
    currentTime  = std::clock() / (double) CLOCKS_PER_SEC;
    times.push_back(frameCount);
}

void AffordanceTime::markCurrentInteractions(double dist, cv::Rect pos,double prob,int frameCount) {
    this->affordance = Affordance(this->affordance.getName(),dist,pos,prob);
    currentTime  = std::clock() / (double) CLOCKS_PER_SEC;
    times.push_back(frameCount);
}

void AffordanceTime::clean() {
    bool check = times.size() > 0;
    if (!check)
        return;
    int lasttime = times.back();

    while(check){
        if (times.empty())
            break;
        int time = times.front();
        int timeDiff = lasttime - time;
        if (timeDiff > 500)
            times.pop_front();
        else
            check = false;
    }
}

bool AffordanceTime::operator ==(const AffordanceTime& autre) {
    if (this == &autre)
        return true;

    return this->affordance == autre.affordance;
}
bool AffordanceTime::operator <(const AffordanceTime& autre) {
    if (this == &autre)
        return false;

    return this->affordance < autre.affordance;
}

