#ifndef WEBCAM_H
#define WEBCAM_H


#include "VideoSource.h"

class WebCam  : public VideoSource{

public:
    WebCam();
    ~WebCam();
    cv::Mat getColorFeed();
    cv::Mat getDepthFeed();
    cv::Mat getOriginalDepth(){return feed;}

    cv::Mat getMappedFeed();
    void update();
    bool hasDepthSource(){ return false;}
    std::string getTimeStamp() {return "<font color=\"#CD2034\">&#149;</font> En direct";}
    int getTimePosition() {return 100;}
    double getExactTimePosition() {return 0;}


private:
    cv::VideoCapture cv_cap;
    cv::Mat feed;


};


#endif //WEBCAM_H
