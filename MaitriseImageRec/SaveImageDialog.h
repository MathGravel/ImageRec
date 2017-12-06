#pragma once
 
#include <string>
#include <sstream>
#include <opencv2/opencv.hpp>
#include "Resource.h"
class SaveImageDialog : public CDialog
{
public:
	enum { IDD = IDD_CATEGORIES_WINDOWS }; 
	SaveImageDialog();
	SaveImageDialog(cv::Mat& image); 
	~SaveImageDialog();
};

