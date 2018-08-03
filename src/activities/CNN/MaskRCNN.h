//
// Created by uqamportable on 12/03/18.
//

#ifndef IMAGEREC_MASKRCNN_H
#define IMAGEREC_MASKRCNN_H

#include <opencv2/dnn.hpp>
#include <vector>
#include <iostream>
#include <fstream>

#include "DetectedObject.h"
#include "Detector.h"

using namespace cv;
using namespace cv::dnn;

class MaskRCNN : public Detector {

public:
    MaskRCNN(std::string inference_path, int imgHeight,int imgWidth, bool estMain,float _prob);
     ~MaskRCNN();
    std::vector<DetectedObject> findObjects(cv::Mat color,cv::Mat depth);
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

};


#endif //IMAGEREC_MASKRCNN_H
