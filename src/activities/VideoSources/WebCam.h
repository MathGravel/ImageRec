//
// Created by uqamportable on 28/01/18.
//

#ifndef IMAGEREC_WEBCAM_H
#define IMAGEREC_WEBCAM_H


#include "VideoSource.h"

class WebCam  : public VideoSource{

public:
    WebCam();
    ~WebCam();
    cv::Mat getColorFeed();
    cv::Mat getDepthFeed();
    cv::Mat getMappedFeed();
    void update();
    bool hasDepthSource(){ return false;};
    std::string getTimeStamp() {return "--/--";}

private:
    cv::VideoCapture cv_cap;
    cv::Mat feed;


};


#endif //IMAGEREC_WEBCAM_H
