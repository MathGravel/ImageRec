//
// Created by uqamportable on 20/06/18.
//

#ifndef IMAGEREC_DETECTOR_H
#define IMAGEREC_DETECTOR_H

#include <vector>
#include "DetectedObject.h"


class Detector {

public :
    virtual std::vector<DetectedObject> indObjects(cv::Mat color, cv::Mat depth) {}

};


#endif //IMAGEREC_DETECTOR_H
