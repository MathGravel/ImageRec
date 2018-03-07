//
// Created by uqamportable on 06/03/18.
//

#include "CNN/HandDetector.h"

HandDetector::HandDetector() {}

HandDetector::~HandDetector() {

}

void HandDetector::train() {}

std::string HandDetector::getPictureInfo(const cv::Mat &image) {}

void HandDetector::updateModel(const cv::Mat &picture, bool correctlyIdentified) {}

std::string HandDetector::predict(const cv::Mat &picture) {}

void HandDetector::savePicture(const cv::Mat &picture, std::string name) {}

void HandDetector::trainCNN() {}

