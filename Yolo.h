//
// Created by uqamportable on 12/03/18.
//

#ifndef IMAGEREC_YOLO_H
#define IMAGEREC_YOLO_H


#include "DetectedObject.h"
#include "Detector.h"


class Yolo : public Detector {

public:
    Yolo(float _prob);
    ~Yolo();
    std::vector<DetectedObject> findObjects(cv::Mat color,cv::Mat depth);

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


#endif //IMAGEREC_MASKRCNN_H
