
// MaitriseImageRec.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "stdafx.h"

// CMaitriseImageRecApp:
// See MaitriseImageRec.cpp for the implementation of this class
//

class CMaitriseImageRecApp : public CWinApp
{
public:
	CMaitriseImageRecApp();

// Overrides
public:
	virtual BOOL InitInstance();
	void ProcessThreadInternal();
	HRESULT InitializeDefaultSensor();
	HBITMAP Testing();

// Implementation
	DECLARE_MESSAGE_MAP()

private :
	// Current Kinect
	IKinectSensor*          m_pKinectSensor;
	ICoordinateMapper*      m_pCoordinateMapper;
	HANDLE hKinectThread;
	// Body reader
	IMultiSourceFrameReader*       m_multiSourceFrame;
	IMultiSourceFrame* m_multiFrame;
	IDepthFrameReference* m_depthFrame;
	IBodyFrameReference* m_bodyFrame;
	IColorFrameReference* m_colorFrame;
	WAITABLE_HANDLE waitableHandle = 0;

	void FrameArrived(IMultiSourceFrameArrivedEventArgs*);
	void ProcessColor(INT64 nTime, RGBQUAD* pBuffer, int nWidth, int nHeight);

};

extern CMaitriseImageRecApp theApp;