//
// Created by uqamportable on 12/03/18.
//

#ifndef IMAGEREC_YOLOGPU_H
#define IMAGEREC_YOLOGPU_H


#include "DetectedObject.h"
#include "Detector.h"
#include "Yolo.h"

class YoloGPU : public Yolo {

public:
    YoloGPU(float _prob);
    ~YoloGPU();
    std::vector<DetectedObject> findObjects(cv::Mat color,cv::Mat depth);
    std::string getDetectorType(){return "GPU";}
    void serialize(std::ostream& stream);
    void deserialize(std::istream& stream);

private:

    char* cfg;
    char *fil ;
    char * wei ;
    char *dat ;
    char *lbl ;
    float nms;
    char **names;
    //list *options;
    char cfgbuff[256];
    char filbuff[256];
    char weibuff[256];
    char datbuff[256];
    char lblbuff[256];
    double time;
    void * net;
    double thresh;
};


#endif //IMAGEREC_YOLOGPU_H
