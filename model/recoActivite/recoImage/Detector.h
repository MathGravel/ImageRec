#ifndef DETECTOR_H
#define DETECTOR_H

#include <vector>
#include "DetectedObject.h"

class Detector {

public :
    virtual std::vector<DetectedObject> findObjects(cv::Mat color, cv::Mat depth) {}


};


#endif //DETECTOR_H
