#ifndef YOLOGPU_H
#define YOLOGPU_H


#include "../DetectedObject.h"
#include "../Detector.h"
#include "Yolo.h"

class YoloGPU : public Yolo {

public:
    YoloGPU(float _prob);
    ~YoloGPU();
    std::vector<DetectedObject> findObjects(cv::Mat color,cv::Mat depth);
    std::string getDetectorType(){return "GPU";}
    void deserialize(std::map<std::string,std::string> stream) {}


private:
    float get_color(int c, int x, int max);

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


#endif //YOLOGPU_H
