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
    MaskRCNN(std::string inference_path, int imgHeight,int imgWidth);
     ~MaskRCNN();
    std::vector<DetectedObject> findObjects(cv::Mat color,cv::Mat depth);

private:

    cv::Mat profondeurs;
    std::vector<DetectedObject> objects;
    std::string network;
    std::string networkDef;
    cv::Mat color_pic;
    cv::Mat originalColor;

    cv::Mat depth_pic;
    cv::dnn::Net neuralNetwork;
    cv::Size cropSize;
    int height;
    int width;
    float resizeRatio;
    float meanRatio;
    float scaleFactor;
    //const char* classes[] = {"Cafetiere","Caffe","Theiere","The","Tasse","Chocolat","Filtre","Main"};
    void classifyImage();
    cv::Mat getProfondeurObjets(cv::Mat img_profon);
    Rect crop;
    Point startingPos;

};


#endif //IMAGEREC_MASKRCNN_H
