#include "RealSenseBag.h"


RealSenseBag::RealSenseBag(std::string bagFile) : bagVideo(std::move(bagFile)) {

    rs2::config config;
    rs2::context context;
    const rs2::playback playback = context.load_device(bagVideo);
    const std::vector<rs2::sensor> sensors = playback.query_sensors();
    for (const rs2::sensor& sensor : sensors) {
		const std::vector<rs2::stream_profile> stream_profiles = sensor.get_stream_profiles();
        for( const rs2::stream_profile& stream_profile : stream_profiles ){
            config.enable_stream( stream_profile.stream_type(), stream_profile.stream_index() );
        }
		
	}
	startTime = std::clock();
	pipe = std::make_shared<rs2::pipeline>();

    // Start Pipeline
    config.enable_device_from_file( playback.file_name() );
    pipeline_profile = pipe->start( config );

    // Set Non Real Time Playback to make sure that no frame is lost.
    // Update : Non real time is broken right now on the software side. Changed to true until a patch
    auto profile = pipeline_profile.get_device().as<rs2::playback>();
    profile.set_real_time(false);

    pipe->wait_for_frames();
	frame = -1;
	maxframe = -1;

    std::cout << "check";
	
    currentDepth = 0;
}

RealSenseBag::~RealSenseBag() {
   //pipe->stop();
   std::exit(42);
}

cv::Mat RealSenseBag::getColorFeed() {
    return this->colorFeed;
}

cv::Mat RealSenseBag::getDepthFeed() {
    return this->depthMeters;
}

cv::Mat RealSenseBag::getMappedFeed() {
    return this->depthMeters;
}

inline void RealSenseBag::update() {



    data = pipe->wait_for_frames();

    auto color_frame = data.get_color_frame();
    assert(color_frame);

    colorFeed = frame_to_mat(color_frame);

    auto depth_frame = data.get_depth_frame();
    assert(depth_frame );
	
    depthFeed = frame_to_mat(depth_frame);
	depthMeters = depth_frame_to_meters(depthFeed);
	currentDepth++;

   frame = std::max(depth_frame.get_frame_number(),color_frame.get_frame_number());
   maxframe = std::max(frame,maxframe);

}


// Converts depth frame to a matrix of doubles with distances in meters
cv::Mat RealSenseBag::depth_frame_to_meters(cv::Mat depthPic) {

#ifndef USE_KITCHEN
    depthPic.convertTo(depthPic, CV_64F);
    depthPic = depthPic * 0.00100000005;

#else

    cv::Mat temp(depthPic.rows,depthPic.cols,CV_64FC1);


    for (int i = 0; i < depthPic.rows;i++) {
        for (int j = 0; j < depthPic.cols; j++) {
            int grayscale = +(depthPic.at<uchar>(i,j));
            double dist = grayscale == 0 ? 0.0 : grayscale > 255 ? 4.0 : grayscale / 64;
            temp.at<double>(i, j) = dist;
        }
    }
    depthPic.release();
    depthPic = temp;

#endif


    return depthPic;

}
/**
 * @brief RealSense::frame_to_mat
 * @param f
 * @return Image matrix
 * Convert a Realsense frame to an Opencv image.
 */
cv::Mat RealSenseBag::frame_to_mat(const rs2::frame& f)
{
    using namespace cv;
    using namespace rs2;


    auto vf = f.as<video_frame>();
    const int w = 1280;//vf.get_width();
    const int h = 720;//vf.get_height();

    if (f.get_profile().format() == RS2_FORMAT_BGR8)
    {
        return Mat(Size(w, h), CV_8UC3, (void*)f.get_data(), Mat::AUTO_STEP);
    }
    else if (f.get_profile().format() == RS2_FORMAT_RGB8)
    {
        auto r = Mat(Size(w, h), CV_8UC3, (void*)f.get_data(), Mat::AUTO_STEP);
        cv::cvtColor(r, r, cv::COLOR_BGR2RGB);
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


