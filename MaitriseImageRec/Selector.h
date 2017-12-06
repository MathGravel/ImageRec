#pragma once

#include <afx.h>
#include <afxwin.h>
#include <opencv2\core.hpp>
#include <opencv2\opencv.hpp>
#include <windef.h>
#include "Dbout.h"

using namespace cv; 
class Selector
{
public:
	Selector();
	~Selector();

	RECT relativePosition(CDC *pDC, CWnd* cwnd, CStatic* cstatic);
	RECT getCursorRectangle(CDC *pDC, CWnd* cwnd, CStatic* cstatic); 
	Mat returnImage(Mat original, RECT relativePos, RECT cursorInfo); 
	Mat getFinalImage(Mat original, RECT capture, CDC *pDC, CWnd* cwnd); 

};

