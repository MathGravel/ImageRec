#ifndef YOLOCPU_H
#define YOLOCPU_H

#include <opencv2/dnn.hpp>
#include <opencv2/opencv.hpp>

#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "../DetectedObject.h"
#include "../Detector.h"
#include "Yolo.h"

#include <map> //Setting area

using namespace cv;
using namespace cv::dnn;

class YoloCPU : public Yolo {

public:
    YoloCPU(float _prob,std::map<std::string,std::string> setting);
    ~YoloCPU();
    std::vector<DetectedObject> findObjects(cv::Mat color,cv::Mat depth);
    std::string getDetectorType(){return "CPU";}
    void deserialize(std::map<std::string,std::string> stream) {}



private:

    std::string* classNames;
    std::vector<DetectedObject> objects;
    std::string network;
    std::string networkDef;
    bool main;
    cv::dnn::Net neuralNetwork;
    cv::Size cropSize;
    int height;
    int width;
    float resizeRatio;
    float confidenceThreshold;
    Rect crop;
    Point startingPos;
    std::vector<String> getOutputsNames(const Net& net);
    float get_color(int c, int x, int max);
    float colors[6][3] = { {255,0,255}, {0,0,255},{0,255,255},{0,255,0},{255,255,0},{255,0,0} };


};


#endif //YOLOCPU_H
