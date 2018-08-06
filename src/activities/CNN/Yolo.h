//
// Created by uqamportable on 02/08/18.
//

#ifndef IMAGEREC_YOLO_H
#define IMAGEREC_YOLO_H

#include <string>
#include "../Detector.h"

class Yolo : public Detector {

    virtual std::string getDetectorType() = 0;
};


#endif //IMAGEREC_YOLO_H
