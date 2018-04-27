//
// Created by uqamportable on 16/02/18.
//

#ifndef IMAGEREC_REALSENSE_H
#define IMAGEREC_REALSENSE_H

#include "VideoSource.h"
#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include <string>
#include <sstream>
#include <iostream>

class RealSense  : public VideoSource{

public:
    RealSense();

    virtual ~RealSense();


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
    rs2::align align_to;
    int last_frame_number;

};

#endif //IMAGEREC_REALSENSE_H
