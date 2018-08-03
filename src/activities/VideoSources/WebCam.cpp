//
// Created by uqamportable on 28/01/18.
//

#include "VideoSources/WebCam.h"


WebCam::WebCam() {
    cv_cap.open(0);
}

WebCam::~WebCam() {
    cv_cap.release();
}

cv::Mat WebCam::getColorFeed() {
    return feed;
}

cv::Mat WebCam::getDepthFeed() {
    return feed;
}

cv::Mat WebCam::getMappedFeed() {
    return feed;
}


void WebCam::update(){

    cv::Mat cv_frame;
    cv_cap.read(cv_frame);
    assert(!cv_frame.empty());
    feed = cv_frame.clone();
}