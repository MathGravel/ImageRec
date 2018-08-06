#ifndef DETECTEDOBJECT_H
#define DETECTEDOBJECT_H

#include <opencv2/opencv.hpp>
#include <map>

typedef std::map<std::string, float> Predictions;

class DetectedMatrice{
public:
    DetectedMatrice(cv::Rect position, Predictions& pre, double _dist ) {
        objPos = position;
        preds = pre;
        dist = _dist;

    }

    const cv::Rect &getObjPos() const {
        return objPos;
    }

    const Predictions& getPredictions() const {
        return preds;
    }

    const float getClassPrediction(const std::string& className) const {
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
    DetectedObject(cv::Rect position, std::string name, double _dist)  {
        objPos = position;
        objName = name;
        dist = _dist;
        prob = 0;
        if (objName == "teapot") {
            objName = "teakettle";
        }
        if(objName == "chocolate")
            objName = "choco";
        if (objName == "pitcher")
            objName = "water";
    }

    DetectedObject(cv::Rect position, std::string name, double _dist, double _prob) {
        objPos = position;
        objName = name;
        dist = _dist;
        prob = _prob;
        if (objName == "teapot") {
            objName = "teakettle";
        }
        if(objName == "chocolate")
            objName = "choco";
        if (objName == "pitcher")
            objName = "water";
    }


    const cv::Rect &getObjPos() const {
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

    friend std::ostream& operator<<(std::ostream& o,const DetectedObject& obj){
        o << obj.objName << " " << obj.prob << std::endl;
        return o;
    }


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

    DetectedObjects() = default;

    DetectedObjects(std::vector<DetectedObject> objets) :objs(std::move(objets)) {};

    const DetectedObject * begin() const
    {
        return & (*objs.begin());
    }
    const DetectedObject * end() const
    {
        return & (*objs.end());
    }

    bool empty() const {
        return objs.empty();
    }

    void clear()  {
        objs.clear();
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


#endif //DETECTEDOBJECT_H
