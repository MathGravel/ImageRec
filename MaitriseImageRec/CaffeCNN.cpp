#include "CaffeCNN.h"

using namespace caffe;  // NOLINT(build/namespaces)
using std::string;






CaffeCNN::CaffeCNN()
{
}


CaffeCNN::~CaffeCNN()
{
}

void CaffeCNN::train() {
		
}

std::string CaffeCNN::getPictureInfo(const cv::Mat & image)
{
	return std::string(""); 
}

void CaffeCNN::updateModel(const cv::Mat & picture, bool correctlyIdentified)
{
}

std::string CaffeCNN::predict(const cv::Mat & picture)
{
	return std::string();
}

void CaffeCNN::savePicture(const cv::Mat & picture, std::string name)
{
}

void CaffeCNN::trainCNN()
{
}

void CaffeCNN::prepareData()
{
}

void CaffeCNN::setImageMean(const std::string mean_loc)
{
}

std::vector<float> CaffeCNN::predire(const cv::Mat & img)
{
	return std::vector<float>();
}

void CaffeCNN::wrapInputLayer(std::vector<cv::Mat>* input_channels)
{
}

void CaffeCNN::preparation(const cv::Mat & img, std::vector<cv::Mat>* input_channels)
{
}
