//
// Created by uqamportable on 28/01/18.
//

#include "KinectTreatment.h"


KinectTreatment::KinectTreatment(VideoSource *feed) {
    segImage = ImageSegmentationManual();
    vidFeed = feed;
}

cv::Mat KinectTreatment::getColorFeed() {
    return segImage.segmentPic(vidFeed->getColorFeed());
}

cv::Mat KinectTreatment::getMappedFeed() { return segImage.segmentPic(vidFeed->getMappedFeed());}

cv::Mat KinectTreatment::getDepthFeed() { return segImage.segmentPic(vidFeed->getDepthFeed());}

void KinectTreatment::update() {vidFeed->update();}

