#include "stdafx.h"
#include "KinectTreatment.h"


KinectTreatment::KinectTreatment()
{
}

void KinectTreatment::setCurrentPicture(HBITMAP kinectBitmap)
{
}

void KinectTreatment::setCurrentPicture(cv::Mat kinectMat)
{
}

void KinectTreatment::clear()
{
}

void KinectTreatment::addBodyJoints()
{
}

void KinectTreatment::processPicture()
{
}

void KinectTreatment::savePictures(std::string location, std::string filename)
{
}

HBITMAP KinectTreatment::getPicture()
{
	return HBITMAP();
}

HBITMAP KinectTreatment::getHandLeft()
{
	return HBITMAP();
}

HBITMAP KinectTreatment::getHandRight()
{
	return HBITMAP();
}

void KinectTreatment::cropPictures()
{
}

void KinectTreatment::normalizePictures()
{
}

void KinectTreatment::extractHands()
{
}

void KinectTreatment::extractHand(int posx, int posy, int posz)
{
}
