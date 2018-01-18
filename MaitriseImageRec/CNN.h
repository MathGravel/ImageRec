#pragma once
#include <opencv2/opencv.hpp>
#include <string>

/* Pair (label, confidence) representing a prediction. */
typedef std::pair<string, float> Prediction;

class CNN {

public:
	virtual void train() = 0;
	virtual std::string getPictureInfo(const cv::Mat& picture) = 0;
	virtual void updateModel(const cv::Mat& picture, bool correctlyIdentified) = 0;
	virtual std::string predict(const cv::Mat& picture) = 0;
	virtual void savePicture(const cv::Mat& picture, std::string name) = 0;
	virtual void trainCNN() = 0;

};