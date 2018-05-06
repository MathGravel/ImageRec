//
// Created by uqamportable on 12/03/18.
//

#ifndef IMAGEREC_DETECTEDOBJECT_H
#define IMAGEREC_DETECTEDOBJECT_H

#include <opencv2/opencv.hpp>
#include <map>

using namespace cv;

typedef std::map<std::string, float> Predictions;


class DetectedMatrice{
public:
    DetectedMatrice(cv::Rect position, Predictions& pre, double _dist ) {
        objPos = position;
        preds = pre;
        dist = _dist;
    }

    const Rect &getObjPos() const {
        return objPos;
    }

    const Predictions& getPredictions() const {
        return preds;
    }

    const float getClassPrediction(std::string className) const {
        return preds.at(className);
    }

    const double& getDist() const {
        return dist;
    }

private:
    cv::Rect objPos;
    Predictions preds;
    double dist;

};



class DetectedObject {
public:
    DetectedObject(cv::Rect position, std::string name, double _dist) {
        objPos = position;
        objName = name;
        dist = _dist;
    }

    DetectedObject(cv::Rect position, std::string name, double _dist, double _prob) {
        objPos = position;
        objName = name;
        dist = _dist;
        prob = _prob;
    }


    const Rect &getObjPos() const {
        return objPos;
    }

    const std::string &getObjName() const {
        return objName;
    }

    const double& getDist() const {
        return dist;
    }

    const double getProb() const {
        return prob;
    }


private:
    cv::Rect objPos;
    std::string objName;
    double dist;
    double prob;

};

struct DetectedMatrices {
    DetectedMatrices(std::vector<DetectedMatrice>& objets) {
        objs = objets;
    };

    const std::vector<DetectedMatrice>& getMatrices() const {
        return objs;
    }

private:
    std::vector<DetectedMatrice> objs;
};

class DetectedObjects {
public:

    DetectedObjects() {

    }

    DetectedObjects(std::vector<DetectedObject>& objets) {
        objs = objets;

    };


    DetectedObject * begin()
    {
        return & (*objs.begin());
    }
    DetectedObject * end()
    {
        return & (*objs.end());
    }

    bool empty() {
        return objs.empty();
    }


    const std::vector<DetectedObject>& getObjects() const {
        return objs;
    }

    const DetectedObject getObject(std::string objName) const {
        DetectedObject nullObj(cv::Rect(0,0,0,0),"Null Object",0);

        for (DetectedObject obj : objs) {
            if (obj.getObjName() == objName)
                return obj;
        }

        return nullObj;
    }

    const DetectedObject getObject(cv::Rect& region) const {
        DetectedObject nullObj(cv::Rect(0,0,0,0),"Null Object",0);

        for (DetectedObject obj : objs) {
            if (obj.getObjPos() == region)
                return obj;
        }

        return nullObj;
    }


    /*DetectedObjects& operator =(const DetectedObjects detect) {
        for (DetectedObject obj : detect.objs) {
            this->objs.push_back(obj);
        }
    }*/



private:
    std::vector<DetectedObject> objs;
};


#endif //IMAGEREC_DETECTEDOBJECT_H
