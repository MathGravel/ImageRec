
/**
* \file Affordance.h
* \author Mathieu Gravel
* \version 
* \date 
* \brief Evaluates the position between an Object and the Hand
* \details Affordance is used to update regulary the position of the Object during *  an interaction. It evaluates the distance to the Hand of an Object and is used *  to add an interaction and its time in the AffordanceTime class.
**/

#ifndef AFFORDANCE_H
#define AFFORDANCE_H

#include <opencv2/opencv.hpp>
#include <ctime>
#include <sstream>
#include <list>


class Affordance {

public:

    /**
    * \fn	Affordance::Affordance(std::string obj , double prob)
    * \brief	Constructor of Affordance	
    * \param	obj	Name of the new Object
    * \param	prob	Confidence rate on the object
    **/
    Affordance(std::string obj = "teacup", double prob=76.7653);

    /**
    * \fn	Affordance::Affordance(std::string obj,double pos,cv::Rect reg,double prob,   double _dist)
    * \brief	Constructor of Affordance
    * \details	
    * \param	obj	String : Name of the new Object
    * \param	pos	double : position of the Object at Creation
    * \param	reg	cv::Rect : OpenCv class for 2D rectangles
    * \param	prob	double : Confidence rate on the object
    * \param	dist	double : Distance between the Hand and the Object 
    **/
    Affordance(std::string obj,double pos,cv::Rect reg,double prob,double _dist = 0);

    /**
     * \fn getName()
     */
    std::string getName() const {return objectName;}

    /**
     * \fn getObjectPosition()
     */
    double getObjectPosition() const {return objectPos;}

    /**
     * \fn getRegion()
     */
    const cv::Rect& getRegion() const {return region;}

    /**
     * \fn getObjectProbability()
     */
    double getObjectProbability() const {return objProb;}

    /**
     * \fn setDist()
     */
    void setDist(double dist) {
        objectPos = dist;
    }

    /**
     * \fn reset()
     */
    void reset() {
        objectPos = -1;
        objProb = 0;
    }

    /**
    * \fn	bool Affordance::operator ==(const Affordance& autre)
    * \brief	Overload of the '==' operator for two Affordance objects
    * \return	True if this == 'autre'
    **/
    bool operator ==(const Affordance& autre);

    /**
    * \fn	bool Affordance::operator <(const Affordance& autre)
    * \brief	Overload of the '<' operator for two Affordance objects
    * \return	True if this < 'autre'	
    **/
    bool operator <(const Affordance& autre);

    /**
     * \fn to_str()
     * \brief 
     * \return
     */
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
    double dist;
    //Date
    //To_String Format (obj)_(x.xx)
    //Si t'a aucune interaction envoie un objet void i.e un appel constructeur vide

    friend std::ostream& operator <<(std::ostream& o, const Affordance a) {
        return o << a.objectName << " " << a.objProb;
    }



};


class AffordanceTime {

public:

    /**
    * \fn	AffordanceTime::AffordanceTime()
    * \brief	Constructor of AffordanceTime
    **/
    AffordanceTime();

    /**
    * \fn	AffordanceTime::AffordanceTime(Affordance aff,int frameCount)
    * \brief	Constructor of AffordanceTime
    * \param	aff	Affordance : Object of type Affordance
    * \param	frameCount	int : Frame time at the creation
    **/
    AffordanceTime(Affordance aff,int frameCount);

    /**
    * \fn	AffordanceTime::markCurrentInteractions(double dist,int frameCount)
    * \brief	Updates the Affordance object during an interaction
    * \details	Updates the Affordance object, called during an interaction
    *	Updates the current time and adds the frame time of the interaction
    * \param	aff	Affordance : Object of type Affordance
    * \param	frameCount	int : Frame time at the creation
    **/
    void markCurrentInteractions(double dist,int frameCount);

    /**
    * \fn	AffordanceTime::markCurrentInteractions(double dist, cv::Rect  pos,double prob,int frameCount)
    * \brief	Updates the Affordance object during an interaction
    * \details	Updates the Affordance object, called during an interaction
    *	Updates the current time and adds the frame time of the interaction
    *	Sets the Affordance object's position and its probability to correspond
    * \param	aff	Affordance : Object of type Affordance
    * \param	frameCount	int : Frame time at the creation
    **/
    void markCurrentInteractions(double dist, cv::Rect pos,double prob,int frameCount);

    /**
     * @brief getInteractionTime
     * @return
     */
    double getInteractionTime() {
        return times.front() - times.back();
    }

    /**
     * @brief getStartTime
     * @return
     */
    double getStartTime() {
        return times.front();
    }


    /**
     * @brief getAffordance
     * @return
     */
    Affordance& getAffordance() {
        return affordance;
    }

    /**
     * @brief getName
     * @return
     */
    std::string getName() const {
        return affordance.getName();
    }

    /**
     * @brief getNumberOfOccurences
     * @return
     */
    int getNumberOfOccurences() {
        return times.size();
    }

    /**
    * \fn	AffordanceTime::clean()
    * \brief	Cleans the time attribute
    **/
    void clean();
    void reset() {times.clear();}

    /**
     * @brief checkAffName
     * @param className
     * @return
     */
    bool checkAffName(const std::string className) const {
        return affordance.getName() == className;
    }

    /**
    * \fn	AffordanceTime::operator ==(const AffordanceTime& autre)
    * \brief	Overload of the '==' operator for two AffordanceTime objects
    * \return	True if this == 'autre'
    **/
    bool operator ==(const AffordanceTime& autre);

    /**
    * \fn	AffordanceTime::operator <(const AffordanceTime& autre)
    * \brief	Overload of the '<' operator for two AffordanceTime objects
    * \return	True if this < 'autre'
    **/
    bool operator <(const AffordanceTime& autre) ;


private:

    Affordance affordance;
    double startTime;
    double currentTime;
    std::list<int> times;




    friend std::ostream& operator <<(std::ostream& o, const AffordanceTime a) {
        return o << a.affordance;
    }

};


#endif //AFFORDANCE_H
