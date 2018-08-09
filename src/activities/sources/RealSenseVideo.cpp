#include "sources/RealSenseVideo.h"


RealSenseVideo::RealSenseVideo(std::string colorFile ,std::string depthFile ) : colorVideo(std::move(colorFile)),depthVideo(std::move(depthFile)) {

    vid = new cv::VideoCapture(colorVideo);
    vidDepth = new cv::VideoCapture(depthVideo);

}

RealSenseVideo::~RealSenseVideo() {
    vid->release();
    vidDepth->release();
    delete vid;
    delete vidDepth;
}

cv::Mat RealSenseVideo::getColorFeed() {
    return this->colorFeed;
}

cv::Mat RealSenseVideo::getDepthFeed() {
    return this->depthFeed;
}

cv::Mat RealSenseVideo::getMappedFeed() {
    return this->depthMeters;
}

void RealSenseVideo::update() {

    *vid >> colorFeed;
    *vidDepth >> depthFeed;

    // Pour accélérer la vidéo...
    /*
    *vid >> colorFeed;
    *vidDepth >> depthFeed;
    *vid >> colorFeed;
    *vidDepth >> depthFeed;
    *vid >> colorFeed;
    *vidDepth >> depthFeed;
    */

    depthMeters = depth_frame_to_meters(depthFeed);

    if (colorFeed.empty()) {
        vid->release();
        delete vid;
        vid = new cv::VideoCapture(colorVideo);
        *vid >> colorFeed;
        vidDepth->release();
        delete vidDepth;
        vidDepth = new cv::VideoCapture(depthVideo);
        *vidDepth >> depthFeed;
        depthMeters = depth_frame_to_meters(depthFeed);
    }
}


std::string RealSenseVideo::getTimeStamp()
{
    int current = (int) vid->get(cv::CAP_PROP_POS_FRAMES) / vid->get(cv::CAP_PROP_FPS);
    int total = (int) vid->get(cv::CAP_PROP_FRAME_COUNT) / vid->get(cv::CAP_PROP_FPS);

    std::stringstream currentSecond;
    currentSecond << std::setw(2) << std::setfill('0') << std::to_string(current%60);

    std::stringstream totalSecond;
    totalSecond << std::setw(2) << std::setfill('0') << std::to_string(total%60);

    return std::to_string(current/60) + ":" + currentSecond.str() + " / " + std::to_string(total/60) + ":" + totalSecond.str();
}


// Converts depth frame to a matrix of doubles with distances in meters
cv::Mat RealSenseVideo::depth_frame_to_meters(cv::Mat depthPic) {

    cv::Mat temp(depthPic.rows,depthPic.cols,CV_64FC1);


    for (int i = 0; i < depthPic.rows;i++) {
        for (int j = 0; j < depthPic.cols; j++) {
            int grayscale = +(depthPic.at<uchar>(i,j));
            double dist = grayscale == 0 ? 0.0 : grayscale > 255 ? 4.0 : grayscale / 64;
            temp.at<double>(i, j) = dist;
        }
    }

    return temp;
}

