#pragma once
#include "CNN.h"
#include <caffe/caffe.hpp>

#include <string>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <iosfwd>
#include <memory>
#include <string>
#include <utility>
#include <vector>

static bool PairCompare(const std::pair<float, int>& lhs,
	const std::pair<float, int>& rhs) {
	return lhs.first > rhs.first;
}


class CaffeCNN : public  CNN
{
public:
	CaffeCNN();
	~CaffeCNN();
	void train();
	std::string getPictureInfo(const cv::Mat& image);
	void updateModel(const cv::Mat& picture, bool correctlyIdentified);
	 std::string predict(const cv::Mat& picture);
	 void savePicture(const cv::Mat& picture, std::string name);
	 void trainCNN();
	void prepareData();
	


private :
	std::string imagesLoc;
	std::string lmdbLoc;
	std::string trainLmdbLoc;
	std::string caffeModelLoc;
	std::string imageMeanLoc;
	std::string solverLoc;
	std::string trainerLoc;
	void prepareImageMean();
	void transformImagesDB();
	void resizeImagesDB();
	void setImageMean(const string mean_loc);
	std::vector<float> predire(const cv::Mat& img);
	void wrapInputLayer(std::vector<cv::Mat>* input_channels);
	void preparation(const cv::Mat& img,
		std::vector<cv::Mat>* input_channels);


};

