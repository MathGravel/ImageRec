//
// Created by uqamportable on 16/04/18.
//

#ifndef IMAGEREC_RCNN_H
#define IMAGEREC_RCNN_H
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include "../../DetectedObject.h"

/* Pair (label, confidence) representing a prediction. */
typedef std::pair<std::string, float> Prediction;

class RCNN {

public:
    virtual void train() = 0;
    virtual void updateModel(const cv::Mat& picture, bool correctlyIdentified) = 0;
    virtual std::vector<DetectedObject> predict(const cv::Mat& picture) = 0;
    virtual void savePicture(const cv::Mat& picture, std::string name) = 0;

};


#endif //IMAGEREC_RCNN_H
