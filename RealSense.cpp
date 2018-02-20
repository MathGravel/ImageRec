//
// Created by uqamportable on 16/02/18.
//

#include "RealSense.h"

RealSense::RealSense() {

    rs2::config cfg;

    //Add desired streams to configuration
    cfg.enable_stream(RS2_STREAM_COLOR, 1920, 1080, RS2_FORMAT_BGR8, 30);

    //cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, 30);
    //cfg.enable_stream(RS2_STREAM_DEPTH,1280,720,RS2_FORMAT_Z16);
    pipe.start(cfg);

    for(int i = 0; i < 30; i++)
    {
        //Wait for all configured streams to produce a frame
        data = pipe.wait_for_frames();
    }

}

RealSense::~RealSense() {

}

cv::Mat RealSense::getColorFeed() {
    return this->colorFeed;
}

cv::Mat RealSense::getDepthFeed() {
    return this->depthFeed;
}

cv::Mat RealSense::getMappedFeed() {
    return this->colorFeed;
}

void RealSense::update() {

    rs2::colorizer color_map;
     data = pipe.wait_for_frames(); // Wait for next set of frames from the camera


    rs2::video_frame color = data.get_color_frame(); // Find the color data

    //rs2::video_frame depth = color_map(data.get_depth_frame()); // Find and colorize the depth data




    //depthFeed.release();
    colorFeed.release();
    // Create depth image
   // depthFeed = cv::Mat( depth.get_height(),
     //                depth.get_width(),
     //               CV_16U,
     //                (uchar *)depth.get_data() );

    // Create color image
    colorFeed = cv::Mat( color.get_height(),
                 color.get_width(),
                         CV_8UC3, (void*)color.get_data());
    depthFeed = colorFeed;
    //cv::resize(colorFeed,colorFeed,cv::Size(1920,1080));


}