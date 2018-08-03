//
// Created by uqamportable on 20/06/18.
//

#ifndef IMAGEREC_DETECTOR_H
#define IMAGEREC_DETECTOR_H

#include <vector>
#include "DetectedObject.h"
#include "Serializable.h"

class Detector : public Serializable {

public :
    virtual std::vector<DetectedObject> findObjects(cv::Mat color, cv::Mat depth) {}


};


#endif //IMAGEREC_DETECTOR_H
