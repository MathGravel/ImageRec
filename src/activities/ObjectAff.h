
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

auto key_selector = [](auto pair){return pair.first;};
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
    
    std::vector<AffordanceTime*> findAffordances(DetectedObjects& regions,
                                                 DetectedObjects& hands,
                                                 ObjectsMat& ObjectsMat,
						bool supAtime);


    std::vector<AffordanceTime*> addNull(ObjectsMat &objectMat, bool supAtime);

    bool currentlyHasAffordances()const {return currentAff;}
    AffordanceTime getObjectAffordance(const std::string object) {
        return affordances.at(object);
    }

    std::vector<AffordanceTime> getAffordances() {
        std::vector<AffordanceTime> values(affordances.size());
        transform(affordances.begin(), affordances.end(), values.begin(), value_selector);
            return values;
    }


    friend std::istream &operator>>(std::istream &is, ObjectAffordances &objaff);


private:
    bool currentAff;
    int frameCount;
    int nbClasses;


    class AffordanceCheck {

    public:
        AffordanceCheck(const DetectedObject& obj, const DetectedObject& _hand) {
            object = obj.getObjPos();
            hand = _hand.getObjPos();
            objectClass = obj.getObjName();
            dist = obj.getDist() - _hand.getDist();
            dist = abs(dist);
            gros = (object & hand).area();
            ens = std::min(hand.area(),object.area());
        }

        AffordanceCheck(Region obj, Region _hand) :dist(0)  {
            object = obj;
            hand = _hand;
            objectClass = "Unknown";
        }

        operator bool() const {
            return (gros > 0.2 * ens) && dist < 0.9 ;
        }
        std::string getObjectType() const { return objectClass;}

    private:
        Region object;
        Region hand;
        std::string objectClass;
        double dist;
        int gros;
        int ens;


    };

    void cleanMatrix();


    std::unordered_map<std::string,AffordanceTime> affordances;
    int numClasses;
    std::unordered_map<std::string,Affordance> confMatrix;
};



#endif //OBJECTAFF_H
