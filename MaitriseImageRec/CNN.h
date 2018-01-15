#pragma once
#include <opencv2/opencv.hpp>
#include <string>


class CNN {

public:
	virtual void train() = 0;
	virtual std::string getPictureInfo(const cv::Mat& picture) = 0;
	virtual void updateModel(const cv::Mat& picture, bool correctlyIdentified) = 0;



};