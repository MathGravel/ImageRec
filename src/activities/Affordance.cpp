/**
* \file Affordance.cpp
* \author Mathieu Gravel
* \version 
* \date 
* \brief Evaluates the position between an Object and the Hand
* \details Affordance is used to update regulary the position of the Object during an interaction. It evaluates the distance to the Hand of an Object and is used to add an interaction and its time in the AffordanceTime class.
**/

#include "Affordance.h"

/**
* \fn	Affordance::Affordance(std::string obj , double prob)
* \brief	Constructor of Affordance
* \detail	
* \param	obj	Name of the new Object
* \param	prob	Confidence rate on the object
**/
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

/**
* \fn	Affordance::Affordance(std::string obj,double pos,cv::Rect reg,double prob, double _dist)
* \brief	Constructor of Affordance
* \detail	
* \param	obj	String : Name of the new Object
* \param	pos	double : position of the Object at Creation
* \param	reg	cv::Rect : OpenCv class for 2D rectangles
* \param	prob	double : Confidence rate on the object
* \param	dist	double : Distance between the Hand and the Object 
**/
Affordance::Affordance(std::string obj,double pos,cv::Rect reg,double prob, double _dist) :
        objectName(obj),objectPos(pos),region(reg),objProb(prob),dist(_dist){
    if (objectName == "teapot") {
        objectName = "teakettle";
    }
    if(objectName == "chocolate")
        objectName = "choco";
    if (objectName == "pitcher")
        objectName = "water";
}

/**
* \fn	bool Affordance::operator ==(const Affordance& autre)
* \brief	Overload of the '==' operator for two Affordance objects
* \return	True if this == 'autre'
**/
bool Affordance::operator ==(const Affordance& autre) {
    if (this == &autre)
        return true;
    cv::Point p1(this->region.x + this->region.width/2,this->region.y + this->region.height/2);

    cv::Point p2(autre.region.x + autre.region.width/2,autre.region.y + autre.region.height/2);

    double dist = cv::norm(p1 - p2);
    return this->objectName == autre.objectName && dist < maxVal;
}

/**
* \fn	bool Affordance::operator <(const Affordance& autre)
* \brief	Overload of the '<' operator for two Affordance objects
* \return	True if this < 'autre'	
**/
bool Affordance::operator <(const Affordance& autre) {
    if (this == &autre)
        return false;
    cv::Point p1(this->region.x + this->region.width/2,this->region.y + this->region.height/2);

    cv::Point p2(autre.region.x + autre.region.width/2,autre.region.y + autre.region.height/2);
    double dist = cv::norm(p1 - p2);
    return this->objectName < autre.objectName || dist > maxVal;
}


/**
* \fn	AffordanceTime::AffordanceTime()
* \brief	Constructor of AffordanceTime
**/
AffordanceTime::AffordanceTime(){
    startTime = std::clock() / (double) CLOCKS_PER_SEC;
    currentTime  = std::clock() / (double) CLOCKS_PER_SEC;
}

/**
* \fn	AffordanceTime::AffordanceTime(Affordance aff,int frameCount)
* \brief	Constructor of AffordanceTime
* \param	aff	Affordance : Object of type Affordance
* \param	frameCount	int : Frame time at the creation
**/
AffordanceTime::AffordanceTime(Affordance aff,int frameCount) {
    startTime = std::clock() / (double) CLOCKS_PER_SEC;
    affordance = aff;
    currentTime  = std::clock() / (double) CLOCKS_PER_SEC;
    times.push_back(frameCount);

}

/**
* \fn	AffordanceTime::markCurrentInteractions(double dist,int frameCount)
* \brief	Updates the Affordance object during an interaction
* \details	Updates the Affordance object, called during an interaction
*	Updates the current time and adds the frame time of the interaction
* \param	aff	Affordance : Object of type Affordance
* \param	frameCount	int : Frame time at the creation
**/
void AffordanceTime::markCurrentInteractions(double dist,int frameCount) {
    this->affordance.setDist(dist);
    currentTime  = std::clock() / (double) CLOCKS_PER_SEC;
    times.push_back(frameCount);
}

/**
* \fn	AffordanceTime::markCurrentInteractions(double dist, cv::Rect pos,double prob,int frameCount)
* \brief	Updates the Affordance object during an interaction
* \details	Updates the Affordance object, called during an interaction
*	Updates the current time and adds the frame time of the interaction
*	Sets the Affordance object's position and its probability to correspond
* \param	aff	Affordance : Object of type Affordance
* \param	frameCount	int : Frame time at the creation
**/
void AffordanceTime::markCurrentInteractions(double dist, cv::Rect pos,double prob,int frameCount) {
    this->affordance = Affordance(this->affordance.getName(),dist,pos,prob);
    currentTime  = std::clock() / (double) CLOCKS_PER_SEC;
    times.push_back(frameCount);
}

/**
* \fn	AffordanceTime::clean()
* \brief	Cleans the time attribute
* \details	
**/
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

/**
* \fn	AffordanceTime::operator ==(const AffordanceTime& autre)
* \brief	Overload of the '==' operator for two AffordanceTime objects
* \return	True if this == 'autre'
**/
bool AffordanceTime::operator ==(const AffordanceTime& autre) {
    if (this == &autre)
        return true;

    return this->affordance == autre.affordance;
}

/**
* \fn	AffordanceTime::operator <(const AffordanceTime& autre)
* \brief	Overload of the '<' operator for two AffordanceTime objects
* \return	True if this < 'autre'
**/
bool AffordanceTime::operator <(const AffordanceTime& autre) {
    if (this == &autre)
        return false;

    return this->affordance < autre.affordance;
}

