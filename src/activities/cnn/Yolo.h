#ifndef YOLO_H
#define YOLO_H

#include <string>
#include "../Detector.h"

class Yolo : public Detector {

    virtual std::string getDetectorType() = 0;
};


#endif //YOLO_H
