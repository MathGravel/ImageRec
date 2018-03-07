//
// Created by uqamportable on 06/03/18.
//

#ifndef IMAGEREC_HANDDETECTOR_H
#define IMAGEREC_HANDDETECTOR_H

#include "CNN.h"



class HandDetector : public  CNN {

public:
    HandDetector();

    virtual ~HandDetector();

private:

    void train();
    std::string getPictureInfo(const cv::Mat& image);
    void updateModel(const cv::Mat& picture, bool correctlyIdentified);
    std::string predict(const cv::Mat& picture);
    void savePicture(const cv::Mat& picture, std::string name);
    void trainCNN();


    cv::Point GetHandLocation(const cv::Mat& picture);


};


#endif //IMAGEREC_HANDDETECTOR_H
