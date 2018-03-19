//
// Created by uqamportable on 12/03/18.
//

#ifndef IMAGEREC_DETECTEDOBJECT_H
#define IMAGEREC_DETECTEDOBJECT_H

#include <opencv2/opencv.hpp>


using namespace cv;

class DetectedObject {
public:
    DetectedObject(cv::Rect position, std::string name) {
        objPos = position;
        objName = name;
    }

    const Rect &getObjPos() const {
        return objPos;
    }

    const std::string &getObjName() const {
        return objName;
    }

private:
    cv::Rect objPos;
    std::string objName;

};


#endif //IMAGEREC_DETECTEDOBJECT_H
