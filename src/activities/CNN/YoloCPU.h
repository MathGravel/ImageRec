//
// Created by uqamportable on 12/03/18.
//

#ifndef IMAGEREC_YOLOCPU_H
#define IMAGEREC_YOLOCPU_H

#include <opencv2/dnn.hpp>
#include <vector>
#include <iostream>
#include <fstream>

#include "DetectedObject.h"
#include "Detector.h"
#include "Yolo.h"

using namespace cv;
using namespace cv::dnn;

class YoloCPU : public Yolo {

public:
    YoloCPU(std::string inference_path, int imgHeight,int imgWidth, bool estMain,float _prob);
    ~YoloCPU();
    std::vector<DetectedObject> findObjects(cv::Mat color,cv::Mat depth);
    std::string getDetectorType(){return "CPU";}
    void serialize(std::ostream& stream);
    void deserialize(std::istream& stream);


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

};


#endif //IMAGEREC_YOLOCPU_H
