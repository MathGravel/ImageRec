#pragma once
#include "CNN.h"

class ImgSegCNN : public  CNN {
public:
	ImgSegCNN();
	~ImgSegCNN();

	void train();
	std::string getPictureInfo(const cv::Mat& image);
	void updateModel(const cv::Mat& picture, bool correctlyIdentified);
	std::string predict(const cv::Mat& picture);
	void savePicture(const cv::Mat& picture, std::string name);
	void trainCNN();
private:
	std::string lastName;
	float lastScore;
};