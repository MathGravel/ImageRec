#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include <map>
#include <list>

#define MAXTHRESHOLD 180
#define MINTHRESHOLD 120
#define USECOLORTHREASH 1
#define USEHISTEQUAL 1

using namespace std;
using namespace cv;

class ImageTreatment
{
public:
	ImageTreatment();
	~ImageTreatment();
	enum ImageFilters { BLUR, MEDIAN,BILFILTER, HISTEQUAL, CANNY,THRESHOLD };

	void setImage(cv::Mat pic);
	void findShapes();
	cv::Mat getOriginalPic();
	const cv::Mat& getFilteredPic();
	const cv::Mat& getFilteredPicWithShapes();

	std::vector<cv::Mat> getShapesAvg();
	std::vector<cv::Mat> getShapes();
	void setThreshhold(int threshMin, int threshMax);

private:
	cv::Mat currentPic;
	cv::Mat filteredPic;
	cv::Mat filteredGreyPic;
	cv::Mat filteredPicWithShapes;

	std::map<ImageFilters, bool> filterUsed;
	std::list<cv::Mat> pastPictures;
	std::vector<cv::Mat> currentShapes;
	int minThreshhold = 100;
	int maxThreshold = 255;


	cv::Mat equalizeColorHistograms(cv::Mat& imgInput);
	cv::Mat threshHoldColors(cv::Mat& imgInput);
	void threshholdGrey(cv::Mat& imgInput);
	void threshholdCanny(cv::Mat& imgInput);

	void extractShapes();
	int minThresh = MINTHRESHOLD;
	int maxThresh = MAXTHRESHOLD;

};

