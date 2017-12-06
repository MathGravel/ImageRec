#include "stdafx.h"
#include "GoogleVisionCNN.h"
//#include <Python.h>


GoogleVisionCNN::GoogleVisionCNN()
{

}
GoogleVisionCNN::~GoogleVisionCNN() {
}

void GoogleVisionCNN::train()
{
	//Not needed with the CNN used in network
}

std::string GoogleVisionCNN::getPictureInfo(const cv::Mat & image)
{

	return lastName;
}

void GoogleVisionCNN::updateModel(const cv::Mat & picture, bool correctlyIdentified)
{
}
