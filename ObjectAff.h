//
// Created by uqamportable on 16/04/18.
//

#ifndef IMAGEREC_OBJECTAFF_H
#define IMAGEREC_OBJECTAFF_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <unordered_map>

#include "Affordance.h"
#include "DetectedObject.h"

using namespace cv;
using namespace cv::dnn;

typedef cv::Rect Region;

auto key_selector = [](auto pair){return pair.first;};
auto value_selector = [](auto pair){return pair.second;};



class ObjectAffordances {
public:
    virtual ~ObjectAffordances();

public:

    ObjectAffordances(int numberClasses = 10);



    std::vector<AffordanceTime*> findAffordances(DetectedObjects& regions,DetectedObjects& hands);
    std::vector<AffordanceTime*> findAffordances(std::vector<DetectedObject> &regions, DetectedObjects &hands);

    void clearCurrentAffordances();

    AffordanceTime getObjectAffordance(std::string object) {
        return affordances.at(object);
    }

    std::vector<AffordanceTime> getAffordances() {
        std::vector<AffordanceTime> values(affordances.size());
        transform(affordances.begin(), affordances.end(), values.begin(), value_selector);
            return values;
    };


    friend std::istream &operator>>(std::istream &is, ObjectAffordances &objaff);


private:


    void setConfusionMatrix(int noClasses,double** matrix);
    bool checkifNewInteraction(DetectedObject & obj);


    int nbClasses;

    class AffordanceCheck {

    public:
        AffordanceCheck(const DetectedObject& obj, const DetectedObject& _hand) {
            object = obj.getObjPos();
            hand = _hand.getObjPos();
            objectClass = obj.getObjName();
            dist = obj.getDist() - _hand.getDist();
            dist = abs(dist);

        }

        AffordanceCheck(Region obj, Region _hand)  {
            object = obj;
            hand = _hand;
            objectClass = "Unknown";
        }

        operator bool() const {
            return ((object & hand).area() > 0) && dist < 1.1 ;
        }
        std::string getObjectType() const { return objectClass;}

    private:
        Region object;
        Region hand;
        std::string objectClass;
        double dist;


    };
    std::unordered_map<std::string,AffordanceTime> affordances;
    int numClasses;
    double** confMatrix;

};

#endif //IMAGEREC_OBJECTAFF_H
