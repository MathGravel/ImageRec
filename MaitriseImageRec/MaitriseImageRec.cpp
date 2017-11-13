
// MaitriseImageRec.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MaitriseImageRec.h"
#include "MaitriseImageRecDlg.h"
#include "iostream"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMaitriseImageRecApp

BEGIN_MESSAGE_MAP(CMaitriseImageRecApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

DWORD ProcessThread(LPVOID pParam) {
	CMaitriseImageRecApp *p = (CMaitriseImageRecApp*) pParam;
	p->ProcessThreadInternal();
	return 0;
}





// CMaitriseImageRecApp construction

CMaitriseImageRecApp::CMaitriseImageRecApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CMaitriseImageRecApp object

CMaitriseImageRecApp theApp;


// CMaitriseImageRecApp initialization

BOOL CMaitriseImageRecApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CMaitriseImageRecDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

/// <summary>
/// Initializes the default Kinect sensor
/// </summary>
/// <returns>indicates success or failure</returns>
HRESULT CMaitriseImageRecApp::InitializeDefaultSensor()
{
	HRESULT hr;

	hr = GetDefaultKinectSensor(&m_pKinectSensor);
	if (FAILED(hr))
	{
		return hr;
	}

	if (m_pKinectSensor)
	{
		// Initialize the Kinect and get coordinate mapper and the body reader
		IMultiSourceFrame* pMultiFrameSource = NULL;

		hr = m_pKinectSensor->Open();

		if (FAILED(hr))
		{
			return false;
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->get_CoordinateMapper(&m_pCoordinateMapper);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->OpenMultiSourceFrameReader(FrameSourceTypes_Depth | FrameSourceTypes_Body | FrameSourceTypes_Color, &m_multiSourceFrame);
		}
		m_multiSourceFrame->SubscribeMultiSourceFrameArrived(&waitableHandle);
		//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ProcessThread, this, 0, NULL);
		this->ProcessThreadInternal();

		/*if (SUCCEEDED(hr))
		{
			hr = m_multiSourceFrame->(&m_multiFrame);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_multiFrame->get_BodyFrameReference(&m_bodyFrame);
		}
		if (SUCCEEDED(hr))
		{
			hr = m_multiFrame->get_DepthFrameReference(&m_depthFrame);
		}
		if (SUCCEEDED(hr))
		{
			hr = m_multiFrame->get_ColorFrameReference(&m_colorFrame);
		}
		IBodyFrame* pBodyFrame = NULL;

		HRESULT hr = m_bodyFrame->AcquireFrame(&pBodyFrame); */





	}
	return hr;
}

	void CMaitriseImageRecApp::FrameArrived(IMultiSourceFrameArrivedEventArgs *pArgs) {
		HRESULT hr;
		IMultiSourceFrameReference *pFrameReference = nullptr;

		//cout << "got a valid frame" << endl;
		hr = pArgs->get_FrameReference(&pFrameReference);
		if (SUCCEEDED(hr))
		{
			IMultiSourceFrame *pFrame = nullptr;
			hr = pFrameReference->AcquireFrame(&pFrame);
			//if (FAILED(hr)) {
			//	std::cout << "fail on AcquireFrame" << std::endl;
			//}
			IColorFrameReference* pColorFrameReference = nullptr;
			IDepthFrameReference* pDepthFrameReference = nullptr;
			IBodyIndexFrameReference* pBodyIndexFrameReference = nullptr;
			hr = pFrame->get_DepthFrameReference(&pDepthFrameReference);
			//if (SUCCEEDED(hr))
				//DepthFrameArrived(pDepthFrameReference);
			SafeRelease(pDepthFrameReference);


			hr = pFrame->get_ColorFrameReference(&pColorFrameReference);
			//if (SUCCEEDED(hr))
			//	ColorFrameArrived(pColorFrameReference);
			SafeRelease(pColorFrameReference);

			hr = pFrame->get_BodyIndexFrameReference(&pBodyIndexFrameReference);
			//if (SUCCEEDED(hr))
			//	BodyIndexFrameArrived(pBodyIndexFrameReference);
			SafeRelease(pBodyIndexFrameReference);

			pFrameReference->Release();
		}
	}
	



void CMaitriseImageRecApp::ProcessThreadInternal() {
	HANDLE handles[] = { reinterpret_cast<HANDLE>(waitableHandle) };
	int idx;
	bool quit = false;
	while (!quit) {
		// Wait for any of the events to be signalled
		idx = WaitForMultipleObjects(1, handles, FALSE, 100);
		switch (idx) {
		case WAIT_TIMEOUT:
			continue;
		case WAIT_OBJECT_0:
			IMultiSourceFrameArrivedEventArgs *pFrameArgs = nullptr;
			HRESULT hr = m_multiSourceFrame->GetMultiSourceFrameArrivedEventData(waitableHandle, &pFrameArgs);
			//frame arrived
			FrameArrived(pFrameArgs);
			pFrameArgs->Release();
			break;

		}

	}
}


HBITMAP CMaitriseImageRecApp::Testing() {

	HRESULT hr;

	CBitmap b;

	hr = GetDefaultKinectSensor(&m_pKinectSensor);
	if (FAILED(hr))
	{
		return NULL;
	}

	if (m_pKinectSensor)
	{
		// Initialize the Kinect and get coordinate mapper and the body reader
		IMultiSourceFrame* pMultiFrameSource = NULL;

		hr = m_pKinectSensor->Open();



		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->get_CoordinateMapper(&m_pCoordinateMapper);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->OpenMultiSourceFrameReader(FrameSourceTypes_Depth | FrameSourceTypes_Body | FrameSourceTypes_Color, &m_multiSourceFrame);
		}

		WAITABLE_HANDLE m_MultiSourceEvent;
		m_multiSourceFrame->SubscribeMultiSourceFrameArrived(&m_MultiSourceEvent);

		while (true)

		{

			if (WAIT_OBJECT_0 == WaitForSingleObject(reinterpret_cast<HANDLE>(m_MultiSourceEvent), INFINITE))
			{

				IMultiSourceFrameArrivedEventArgs* pArgs{ nullptr };
				IMultiSourceFrameReference* frameRef{ nullptr };
				IMultiSourceFrame* frame{ nullptr };
				//S_OK
				hr = m_multiSourceFrame->GetMultiSourceFrameArrivedEventData(m_MultiSourceEvent, &pArgs);
				//S_OK
				hr = pArgs->get_FrameReference(&frameRef);
				//S_OK
				hr = frameRef->AcquireFrame(&frame);
				IColorFrameReference *colorFrameRef{ nullptr };
				//S_OK
				hr = frame->get_ColorFrameReference(&colorFrameRef);
				IColorFrame *colorFrame = nullptr;
				// Next one fails with E_FAIL
				hr = colorFrameRef->AcquireFrame(&colorFrame);

				//------------------------------

				if (SUCCEEDED(hr))
				{
					INT64 nTime = 0;
					IFrameDescription* pFrameDescription = NULL;
					int nWidth = 0;
					int nHeight = 0;
					ColorImageFormat imageFormat = ColorImageFormat_None;
					UINT nBufferSize = 0;
					RGBQUAD *pBuffer = NULL;

					hr = colorFrame->get_RelativeTime(&nTime);

					if (SUCCEEDED(hr))
					{
						hr = colorFrame->get_FrameDescription(&pFrameDescription);
					}

					if (SUCCEEDED(hr))
					{
						hr = pFrameDescription->get_Width(&nWidth);
					}

					if (SUCCEEDED(hr))
					{
						hr = pFrameDescription->get_Height(&nHeight);
					}

					if (SUCCEEDED(hr))
					{
						hr = colorFrame->get_RawColorImageFormat(&imageFormat);
					}

					if (SUCCEEDED(hr))
					{
						if (imageFormat == ColorImageFormat_Bgra)
						{
							hr = colorFrame->AccessRawUnderlyingBuffer(&nBufferSize, reinterpret_cast<BYTE**>(&pBuffer));

							CDC sdc;
							sdc.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
							CDC dc;
							dc.CreateCompatibleDC(&sdc);
							CBitmap b;
							b.CreateCompatibleBitmap(&sdc, nWidth, nHeight);
							b.SetBitmapBits(nBufferSize, reinterpret_cast<BYTE**>(&pBuffer));
							return (HBITMAP)b.Detach();
						}
						else if (imageFormat == ColorImageFormat_Yuy2) {
							RGBQUAD* m_pColorRGBX = new RGBQUAD[nWidth * nHeight];
							pBuffer = m_pColorRGBX;
							nBufferSize = nWidth * nHeight * sizeof(RGBQUAD);
							hr = colorFrame->CopyConvertedFrameDataToArray(nBufferSize, reinterpret_cast<BYTE*>(pBuffer), ColorImageFormat_Bgra);


							CDC sdc;
							sdc.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
							CDC dc;
							dc.CreateCompatibleDC(&sdc);
							CBitmap bmp;
							bmp.CreateCompatibleBitmap(&sdc, nWidth, nHeight);
							bmp.SetBitmapBits(nBufferSize, reinterpret_cast<BYTE*>(pBuffer));


							return (HBITMAP)bmp.Detach();

							

						}

						//else
						//{
						//	hr = E_FAIL;
						//}
					}

					//if (SUCCEEDED(hr))
					//{
					//	ProcessColor(nTime, pBuffer, nWidth, nHeight);
					//}

					SafeRelease(pFrameDescription);
				}



				//----------------

				SafeRelease(colorFrame);
				SafeRelease(colorFrameRef);
				SafeRelease(frameRef);

				SafeRelease(pArgs);
			}

		} // end main loop
	}
}
/*
/// <summary>
/// Handle new color data
/// <param name="nTime">timestamp of frame</param>
/// <param name="pBuffer">pointer to frame data</param>
/// <param name="nWidth">width (in pixels) of input image data</param>
/// <param name="nHeight">height (in pixels) of input image data</param>
/// </summary>
void CMaitriseImageRecApp::ProcessColor(INT64 nTime, RGBQUAD* pBuffer, int nWidth, int nHeight)
{
	if (m_hWnd)
	{
		if (!m_nStartTime)
		{
			m_nStartTime = nTime;
		}

		double fps = 0.0;

		LARGE_INTEGER qpcNow = { 0 };
		if (m_fFreq)
		{
			if (QueryPerformanceCounter(&qpcNow))
			{
				if (m_nLastCounter)
				{
					m_nFramesSinceUpdate++;
					fps = m_fFreq * m_nFramesSinceUpdate / double(qpcNow.QuadPart - m_nLastCounter);
				}
			}
		}

		WCHAR szStatusMessage[64];
		StringCchPrintf(szStatusMessage, _countof(szStatusMessage), L" FPS = %0.2f    Time = %I64d", fps, (nTime - m_nStartTime));

		if (SetStatusMessage(szStatusMessage, 1000, false))
		{
			m_nLastCounter = qpcNow.QuadPart;
			m_nFramesSinceUpdate = 0;
		}
	}

	// Make sure we've received valid data
	if (pBuffer && (nWidth == cColorWidth) && (nHeight == cColorHeight))
	{
		// Draw the data with Direct2D
		m_pDrawColor->Draw(reinterpret_cast<BYTE*>(pBuffer), cColorWidth * cColorHeight * sizeof(RGBQUAD));

		if (m_bSaveScreenshot)
		{
			WCHAR szScreenshotPath[MAX_PATH];

			// Retrieve the path to My Photos
			GetScreenshotFileName(szScreenshotPath, _countof(szScreenshotPath));

			// Write out the bitmap to disk
			HRESULT hr = SaveBitmapToFile(reinterpret_cast<BYTE*>(pBuffer), nWidth, nHeight, sizeof(RGBQUAD) * 8, szScreenshotPath);

			WCHAR szStatusMessage[64 + MAX_PATH];
			if (SUCCEEDED(hr))
			{
				// Set the status bar to show where the screenshot was saved
				StringCchPrintf(szStatusMessage, _countof(szStatusMessage), L"Screenshot saved to %s", szScreenshotPath);
			}
			else
			{
				StringCchPrintf(szStatusMessage, _countof(szStatusMessage), L"Failed to write screenshot to %s", szScreenshotPath);
			}

			SetStatusMessage(szStatusMessage, 5000, true);

			// toggle off so we don't save a screenshot again next frame
			m_bSaveScreenshot = false;
		}
	}
}*/

