//
// Created by uqamportable on 16/02/18.
//

#include "RealSense.h"

RealSense::RealSense() :align_to(RS2_STREAM_COLOR) {

    rs2::config cfg;

    //Add desired streams to configuration
    cfg.enable_stream(RS2_STREAM_COLOR, 1280, 720, RS2_FORMAT_BGR8, 30);

    //cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, 30);
    cfg.enable_stream(RS2_STREAM_DEPTH,1280,720,RS2_FORMAT_Z16);
    //pipe.start(cfg);
    last_frame_number = 0;
    auto config = pipe.start(cfg);
    auto profile = config.get_stream(RS2_STREAM_COLOR)
            .as<rs2::video_stream_profile>();

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

     data = pipe.wait_for_frames(); // Wait for next set of frames from the camera
    data = align_to.process(data);



    auto color_frame = data.get_color_frame();
    //auto depth_frame = data.get_depth_frame();
    rs2::frame depth = color_map(data.get_depth_frame());





    // If we only received new depth frame,
    // but the color did not update, contcolorFeedinue
    if (color_frame.get_frame_number() == last_frame_number) return;
    last_frame_number = color_frame.get_frame_number();

    //colorFeed.release();
    colorFeed = this->frame_to_mat(color_frame);


    // Query frame size (width and height)
    const int w = depth.as<rs2::video_frame>().get_width();
    const int h = depth.as<rs2::video_frame>().get_height();

    depthFeed.release();

    //colorFeed = frame_to_mat(depth);
    depthFeed =  cv::Mat(cv::Size(w, h), CV_8UC3, (void*)depth.get_data(), cv::Mat::AUTO_STEP);

    cv::resize(colorFeed,colorFeed,cv::Size(1920,1080));
    cv::resize(depthFeed,depthFeed,cv::Size(1920,1080));


}

cv::Mat RealSense::frame_to_mat(const rs2::frame& f)
{
    using namespace cv;
    using namespace rs2;

    auto vf = f.as<video_frame>();
    const int w = vf.get_width();
    const int h = vf.get_height();

    if (f.get_profile().format() == RS2_FORMAT_BGR8)
    {
        return Mat(Size(w, h), CV_8UC3, (void*)f.get_data(), Mat::AUTO_STEP);
    }
    else if (f.get_profile().format() == RS2_FORMAT_RGB8)
    {
        auto r = Mat(Size(w, h), CV_8UC3, (void*)f.get_data(), Mat::AUTO_STEP);
        cv::cvtColor(r, r, CV_BGR2RGB);
        return r;
    }
    else if (f.get_profile().format() == RS2_FORMAT_Z16)
    {
        return Mat(Size(w, h), CV_16UC1, (void*)f.get_data(), Mat::AUTO_STEP);
    }
    else if (f.get_profile().format() == RS2_FORMAT_Y8)
    {
        return Mat(Size(w, h), CV_8UC1, (void*)f.get_data(), Mat::AUTO_STEP);;
    }

    throw std::runtime_error("Frame format is not supported yet!");
}

// Converts depth frame to a matrix of doubles with distances in meters
cv::Mat RealSense::depth_frame_to_meters(const rs2::pipeline& pipe, const rs2::depth_frame& f)
{
    using namespace cv;
    using namespace rs2;

    Mat dm = frame_to_mat(f);
    dm.convertTo(dm, CV_64F);
    auto depth_scale = pipe.get_active_profile()
            .get_device()
            .first<depth_sensor>()
            .get_depth_scale();
    dm = dm * depth_scale;
    return dm;
}