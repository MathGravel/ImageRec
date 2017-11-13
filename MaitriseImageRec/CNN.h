#pragma once

#include "stdafx.h"
#include "opencv2\opencv.hpp"
#include <string>


class CNN {

public:
	CNN();
	void train(std::string dataLocation);
	void getPictureInfo(cv::Mat picture);


private:
	void extractLayer(int layer);
	void trainInternal(std::string dataLocation);


};