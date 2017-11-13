#pragma once


#include "resource.h"		// main symbols
#include "stdafx.h"
#include "opencv2\opencv.hpp"
#include <string>


class KinectTreatment {

public:
	KinectTreatment();

	void setCurrentPicture(HBITMAP kinectBitmap);
	void setCurrentPicture(cv::Mat kinectMat);
	void clear();
	void addBodyJoints();
	void processPicture();
	void savePictures(std::string location, std::string filename);

	HBITMAP getPicture();
	HBITMAP getHandLeft();
	HBITMAP getHandRight();

private:
	void cropPictures();
	void normalizePictures();
	void extractHands();
	void extractHand(int posx, int posy, int posz = 0);


};
