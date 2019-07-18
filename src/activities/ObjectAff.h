
/**
* \file ObjectAff.h
* \author Mathieu Gravel
* \version 
* \date 
* \brief 
* \details
**/

#ifndef OBJECTAFF_H
#define OBJECTAFF_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <unordered_map>

#include "Affordance.h"
#include "DetectedObject.h"
#include "ObjectsMat.h"

using namespace cv;

typedef cv::Rect Region;

//auto key_selector = [](auto pair){return pair.first;};
auto value_selector = [](auto pair){return pair.second;};


class ObjectAffordances {
public:
    virtual ~ObjectAffordances();

public:
    /**
    * \fn	ObjectAffordances(int numberClasses = 10)
    * \brief	Constructor of ObjectAffordances	
    * \details initializes numClasses with the given parameter, sets frameCount and nbClasses to 0, sets currentAff to false
    * \param	int numberClasses
    **/
    ObjectAffordances(int numberClasses = 10);
    
    /**
    * \fn	clearCurrentAffordances();
    * \brief	clears current affordances of the object
    * \details calls the clear() method of affordances, sets frameCount to 0
    **/
    void clearCurrentAffordances();
    
    /**
    * \fn	findAffordances(DetectedObjects& regions,DetectedObjects& hands,ObjectsMat& ObjectsMat, bool supAtime)
    * \brief finds the affordances between the hands and the different objects
    * \details Compares the distance between each objects and the hands. If there are affordances, returnes the vector of AffordanceTime
    * \param DetectedObjects& regions : object from the class DetectedObjects, vector of objects detected on the screen besides Hands
    * \param DetectedObjects& hands : object from the class DetectedObjects, vector of hands detected on the screen
    * \param ObjectsMat& ObjectsMat : object having a vector of AffordanceTime
    * \param bool supAtime : boolean, indicates if more than a certain time passed since the start of the video
    **/
    std::vector<AffordanceTime*> findAffordances(DetectedObjects& regions,
                                                 DetectedObjects& hands,
                                                 ObjectsMat& ObjectsMat,
						bool supAtime);


    /**
    * \fn	addNull(ObjectsMat &objectMat, bool supAtime)
    * \brief adds a null affordance if no hands and no objects are detected
    * \details Creates a null affordance, adds it to the vector of affordances currentAffordances, updates objectMat with the updated vector of affordances and returns currentAffordances
    * \param ObjectsMat &objectMat : object from the class ObjectMat
    * \param bool supAtime : boolean, indicates if more than a certain time passed since the start of the video
    **/
    std::vector<AffordanceTime*> addNull(ObjectsMat &objectMat, bool supAtime);

    //bool currentlyHasAffordances()const {return currentAff;}

    /**
    * \fn	getObjectAffordance(const std::string object)
    **/
    AffordanceTime getObjectAffordance(const std::string object) {
        return affordances.at(object);
    }

    /**
    * \fn	getAffordances()
    **/
    std::vector<AffordanceTime> getAffordances() {
        std::vector<AffordanceTime> values(affordances.size());
        transform(affordances.begin(), affordances.end(), values.begin(), value_selector);
            return values;
    }

    /**
     * @brief operator >>
     * @param is
     * @param objaff
     * @return
     */
    friend std::istream &operator>>(std::istream &is, ObjectAffordances &objaff);


private:
    //bool currentAff;
    int frameCount;
    int nbClasses;


    class AffordanceCheck {

    public:
        /**
        * \fn	AffordanceCheck(const DetectedObject& obj, const DetectedObject& _hand)
        * \brief	Constructor of AffordanceCheck
        * \details  sets the object and the hand position, sets the  Name of the object and the distance between the hand and the object
        * \param    const DetectedObject& obj : object given as parameter
        * \param    const DetectedObject& _hand : hand given as parameter
        **/
        AffordanceCheck(const DetectedObject& obj, const DetectedObject& _hand) {
            object = obj.getObjPos();
            hand = _hand.getObjPos();
            objectClass = obj.getObjName();
            dist = obj.getDist() - _hand.getDist();
            dist = abs(dist);
            gros = (object & hand).area();
            ens = std::min(hand.area(),object.area());
        }

        /**
        * \fn	AffordanceCheck(Region obj, Region _hand) :dist(0)
        * \brief	Constructor of AffordanceCheck
        * \details  sets the hand and the object for an unknown object, sets the distance to 0.
        * \param    Region obj : object given as parameter
        * \param    Region _hand : hand given as parameter
        **/
        AffordanceCheck(Region obj, Region _hand) :dist(0)  {
            object = obj;
            hand = _hand;
            objectClass = "Unknown";
        }

        /**
         * @brief operator bool
         */
        operator bool() const {
            return (gros > 0.2 * ens) && dist < 0.9 ;
        }

        /**
         * @brief getObjectType
         * @return
         */
        std::string getObjectType() const { return objectClass;}

    private:
        Region object;
        Region hand;
        std::string objectClass;
        double dist;
        int gros;
        int ens;


    };

    /**
     * @brief cleanMatrix
     */
    void cleanMatrix();


    std::unordered_map<std::string,AffordanceTime> affordances;
    int numClasses;
    std::unordered_map<std::string,Affordance> confMatrix;
};



#endif //OBJECTAFF_H
