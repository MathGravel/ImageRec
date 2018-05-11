//
// Created by uqamportable on 11/05/18.
//

#ifndef IMAGEREC_REALSENSEVIDEO_H
#define IMAGEREC_REALSENSEVIDEO_H


#include "VideoSource.h"
#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

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
    cv::Mat frame_to_mat(const rs2::frame& f);
    cv::Mat depth_frame_to_meters(const rs2::pipeline& pipe, const rs2::depth_frame& f);
    cv::Mat depth_frame_limit(const rs2::pipeline& pipe, const rs2::depth_frame& f);

    cv::Mat colorFeed;
    cv::Mat depthFeed;
    cv::Mat depthMeters;

    rs2::colorizer color_map;
    rs2::pipeline pipe;
    rs2::frameset data;
    cv::VideoCapture* vid;
    rs2::align align_to;
    int last_frame_number;

};


#endif //IMAGEREC_REALSENSEVIDEO_H
