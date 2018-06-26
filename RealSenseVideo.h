//
// Created by uqamportable on 11/05/18.
//

#ifndef IMAGEREC_REALSENSEVIDEO_H
#define IMAGEREC_REALSENSEVIDEO_H


#include "VideoSource.h"

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <gtkmm/filechooserdialog.h>
#include <string>
#include <sstream>
#include <iostream>


class RealSenseVideo  : public VideoSource{

public:
    RealSenseVideo();

    virtual ~RealSenseVideo();


    cv::Mat getColorFeed();
    cv::Mat getDepthFeed();
    cv::Mat getMappedFeed();
    void update();
private:
    //A faire, ajouter les composants necessaire pour utiliser la camera realsense.
    cv::Mat depth_frame_to_meters(cv::Mat depthPic);

    cv::Mat colorFeed;
    cv::Mat depthFeed;
    cv::Mat depthMeters;

    cv::VideoCapture* vid;
    cv::VideoCapture* vidDepth;
    std::string colorVideo;
    std::string depthVideo;

};


#endif //IMAGEREC_REALSENSEVIDEO_H
