#include "stdafx.h"
#include "Selector.h"


Selector::Selector()
{
}


Selector::~Selector()
{
}

RECT Selector::relativePosition(CDC * pDC, CWnd * cwnd, CStatic * cstatic)
{
	return RECT();
}

RECT Selector::getCursorRectangle(CDC * pDC, CWnd * cwnd, CStatic * cstatic)
{
	return RECT();
}

Mat Selector::returnImage(Mat original, RECT relativePos, RECT cursorInfo)
{

	return Mat();
}

Mat Selector::getFinalImage(Mat original, RECT capture, CDC * pDC, CWnd * cwnd)
{
	return Mat();
}
