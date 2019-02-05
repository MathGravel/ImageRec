#ifndef DETECTOR_H
#define DETECTOR_H

#include <vector>
#include "DetectedObject.h"
#include "Serializable.h"

class Detector : public Serializable {

public :
    virtual std::vector<DetectedObject> findObjects(cv::Mat color, cv::Mat depth) {}


};


#endif //DETECTOR_H
