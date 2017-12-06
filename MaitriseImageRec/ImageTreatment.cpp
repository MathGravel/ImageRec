#include "stdafx.h"
#include "ImageTreatment.h"


ImageTreatment::ImageTreatment()
{
	filterUsed[BLUR] = true;
	filterUsed[BILFILTER] = false;
	filterUsed[HISTEQUAL] = true;
	filterUsed[CANNY] = false;
	filterUsed[THRESHOLD] = true;
	filterUsed[MEDIAN] = true;

}


ImageTreatment::~ImageTreatment()
{
}

void ImageTreatment::setImage(cv::Mat pic)
{
	this->currentPic = pic;
	if (pastPictures.size() > 4) {
		pastPictures.pop_back();
		pastPictures.push_front(pic);
	}
}



void ImageTreatment::findShapes()
{
	currentShapes.clear();
	filteredPic = currentPic.clone();
	if (filterUsed[MEDIAN])
		medianBlur(filteredPic, filteredPic, 3);
	if (filterUsed[BLUR])
		blur(filteredPic, filteredPic, cv::Size(3, 3));
	if (filterUsed[BILFILTER])
		cv::bilateralFilter(filteredPic, filteredPic, 15, 30, 30, cv::BORDER_DEFAULT);

	extractShapes();

}

cv::Mat ImageTreatment::getOriginalPic()
{
	return this->currentPic;
}

const cv::Mat & ImageTreatment::getFilteredPic()
{
	return this->filteredPic;
}

const cv::Mat & ImageTreatment::getFilteredPicWithShapes()
{
	return this->filteredPicWithShapes;
}

std::vector<cv::Mat> ImageTreatment::getShapesAvg()
{
	return std::vector<cv::Mat>();
}

std::vector<cv::Mat> ImageTreatment::getShapes()
{
	return currentShapes;
}

void ImageTreatment::setThreshhold(int threshMin, int threshMax)
{
	minThresh = threshMin;
	maxThresh = threshMax;
}

cv::Mat ImageTreatment::equalizeColorHistograms(cv::Mat& imgInput)
{

	if (imgInput.channels() >= 3)
	{
		cv::Mat ycrcb;

		cvtColor(imgInput, ycrcb, CV_BGR2YCrCb);
		std::vector<cv::Mat> channels;
		split(ycrcb, channels);

		equalizeHist(channels[0], channels[0]);
		equalizeHist(channels[1], channels[1]);
		equalizeHist(channels[2], channels[2]);
		

		cv::Mat result;
		merge(channels, ycrcb);

		cvtColor(ycrcb, result, CV_BGR2RGB);

		return result;
	}
	return imgInput;
}

cv::Mat ImageTreatment::threshHoldColors(cv::Mat & imgInput)
{
	if (imgInput.channels() >= 3)
	{
		cv::Mat ycrcb;

		cvtColor(imgInput, ycrcb, CV_BGR2YCrCb);
		std::vector<cv::Mat> channels;
		split(ycrcb, channels);

		
		threshold(channels[0], channels[0], 120, 250, cv::THRESH_BINARY);
		threshold(channels[2], channels[2], 120, 250, cv::THRESH_BINARY);

		cv::Mat result;
		merge(channels, ycrcb);

		cvtColor(ycrcb, result, CV_BGR2RGB);

		return result;
	}
	return imgInput;
}

void ImageTreatment::threshholdGrey(cv::Mat & imgInput)
{
	//Actuellement vide 
}

void ImageTreatment::threshholdCanny(cv::Mat & imgInput)
{
	//Actuellement vide 
}

void ImageTreatment::extractShapes()
{

	cv::Mat threshold_output;

	std::vector<std::vector<cv::Point> > contours;

	std::vector<cv::Vec4i> hierarchy;
	cv::Mat dst = filteredPic.clone();;
	

	if (USECOLORTHREASH) {
		dst = threshHoldColors(dst);
	}
	if (USEHISTEQUAL)
		dst = threshHoldColors(dst);

	cvtColor(dst, filteredGreyPic, cv::COLOR_BGR2GRAY);

	threshold(filteredGreyPic, threshold_output, minThresh, maxThresh, cv::THRESH_BINARY);
	findContours(threshold_output, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	std::vector<std::vector<cv::Point> > contours_poly(contours.size());

	//![allthework]

	std::vector<cv::Rect> boundRect(contours.size());

	for (size_t i = 0; i < contours.size(); i++)

	{

		approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 3, true);
		cv::Rect bounding = cv::boundingRect(cv::Mat(contours_poly[i]));
		if (bounding.area()  < (0.8 * (filteredGreyPic.cols * filteredGreyPic.rows)))
			boundRect[i] = bounding;



	}

	std::list<cv::Rect> boundRectNew;

	
	for (int i = 0; i < boundRect.size(); i++)
		if (boundRect[i].area() > 10)
			boundRectNew.push_back(boundRect[i]);

	bool verif = true;
	vector<Rect> boundRectNewer;

	int c = 0;

	list<Rect>::iterator it = boundRectNew.begin();

	while (it != boundRectNew.end()) {
		verif = false;
		list<Rect>::iterator itt = it;
		itt++;
		while (itt != boundRectNew.end()) {
			if ((*it & *itt).area() > 0) {
				Rect temp = *it | *itt;
				boundRectNew.remove(*itt);
				boundRectNew.remove(*it);
				boundRectNew.push_front(temp);
				it = boundRectNew.begin();
				verif = true;
				itt = it;
				itt++;
				continue;
			}
			else
				itt++;
		}
		if (!verif)
			it++;
		std::cout << " The size : " << boundRectNew.size();

	}

	for (it = boundRectNew.begin(); it != boundRectNew.end(); it++) {
		currentShapes.push_back(currentPic(*it));
	}

	filteredPicWithShapes = filteredPic.clone();
	for (size_t i = 0; i < boundRectNewer.size(); i++)
	{
		Scalar color = Scalar(0, 200, 0);

		rectangle(filteredPicWithShapes, boundRectNewer[i].tl(), boundRectNewer[i].br(), color, 2, 8, 0);

	}

}
