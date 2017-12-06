#pragma once
#include "CNN.h"
#include <boost/python.hpp>
#include <boost/foreach.hpp> 

namespace py = boost::python;

class GoogleVisionCNN : public  CNN {
	GoogleVisionCNN();
	~GoogleVisionCNN();

	void train();
	std::string getPictureInfo(const cv::Mat& image);
	void updateModel(const cv::Mat& picture, bool correctlyIdentified);

private :
	std::string lastName;
	float lastScore;
};