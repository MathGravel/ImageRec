#include "../../Header files/stdafx.h"
#include "../../Header files/CNN/ImgSegCNN.h"

ImgSegCNN::ImgSegCNN()
{
}

ImgSegCNN::~ImgSegCNN()
{
}

void ImgSegCNN::train()
{
}

std::string ImgSegCNN::getPictureInfo(const cv::Mat & image)
{
	return std::string();
}

void ImgSegCNN::updateModel(const cv::Mat & picture, bool correctlyIdentified)
{
}

std::string ImgSegCNN::predict(const cv::Mat & picture)
{
	return std::string();
}

void ImgSegCNN::savePicture(const cv::Mat & picture, std::string name)
{
}

void ImgSegCNN::trainCNN()
{
}
