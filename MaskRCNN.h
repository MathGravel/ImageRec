//
// Created by uqamportable on 12/03/18.
//

#ifndef IMAGEREC_MASKRCNN_H
#define IMAGEREC_MASKRCNN_H

#include <opencv2/dnn.hpp>
#include <vector>

#include "DetectedObject.h"

using namespace cv;
using namespace cv::dnn;

class MaskRCNN {

public:
    MaskRCNN(std::string inference_path, int imgHeight,int imgWidth, bool estMain,float _prob);
     ~MaskRCNN();
    std::vector<DetectedObject> findObjects(cv::Mat color,cv::Mat depth);

private:

    std::string classNames[10]  = {"Background","teapot","can","chocolate","tea","coffee","coffeemaker","mug","pitcher","milk"};
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
