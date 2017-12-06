
// MaitriseImageRecDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MaitriseImageRecDlg.h"
#include "afxdialogex.h"
#include "iostream"
#include "sstream"
#include <math.h>
#include <process.h>
#include "Dbout.h"

# include <chrono>

using namespace std;
using  ns = chrono::nanoseconds;
using get_time = chrono::steady_clock;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int WM_FIND = RegisterWindowMessage(L"PICUPDATED");

void mouse_move(int event, int x, int y, int flag, void* param);

bool isDragged = false;
int oldX(0), oldY(0), newX(0), newY(0);
cv::Rect zone(0, 0, 0, 0);
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMaitriseImageRecDlg dialog



CMaitriseImageRecDlg::CMaitriseImageRecDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MAITRISEIMAGEREC_DIALOG, pParent)
	
{


	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	// TODO: Add your control notification handler code here

	//CDialogEx::OnCancel();
	// TODO: Add your control notification handler code here
	
	//PostMessageA(this->m_hWnd, WM_FIND, (WPARAM)1, (LPARAM)2);


}

void CMaitriseImageRecDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PIC, test);
}

BEGIN_MESSAGE_MAP(CMaitriseImageRecDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMaitriseImageRecDlg::OnBnClickedOk)
	ON_REGISTERED_MESSAGE(WM_FIND,CMaitriseImageRecDlg::OnImageUpdated)
	//ON_BN_CLICKED(IDCANCEL, &CMaitriseImageRecDlg::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_CATEGORIE_EDIT, &CMaitriseImageRecDlg::OnEnChangeCategorieEdit)
	ON_BN_CLICKED(IDC_CATEGORIE_SAVE, &CMaitriseImageRecDlg::OnBnClickedCategorieSave)
	ON_BN_CLICKED(IDC_SHOW_FILTER, &CMaitriseImageRecDlg::OnBnClickedShowFilter)

	ON_BN_CLICKED(IDC_PAUSE_CAPTURE, &CMaitriseImageRecDlg::OnBnClickedPauseCapture)
	ON_BN_CLICKED(IDC_RESTART_CAPTURE, &CMaitriseImageRecDlg::OnBnClickedRestartCapture)
	ON_BN_CLICKED(IDC_FORCE_SQUARE, &CMaitriseImageRecDlg::OnBnClickedForceSquare)
	ON_BN_CLICKED(ID_TEST_IMAGE_FIXE, &CMaitriseImageRecDlg::OnBnClickedTestImageFixe)
	ON_BN_CLICKED(IDC_NEURAL_LOCAL, &CMaitriseImageRecDlg::OnBnClickedNeuralLocal)
	ON_BN_CLICKED(IDC_NEURAL_GLOBAL, &CMaitriseImageRecDlg::OnBnClickedNeuralGlobal)
END_MESSAGE_MAP()


// CMaitriseImageRecDlg message handlers

BOOL CMaitriseImageRecDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	cvNamedWindow("IDC_STATIC_OUTPUT", 0);

	cvResizeWindow("IDC_STATIC_OUTPUT", 1920, 1080);
	CWnd* pic = GetDlgItem(IDC_PIC_KINECT);

	HWND hWnd = (HWND)cvGetWindowHandle("IDC_STATIC_OUTPUT");

	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, pic->m_hWnd);


	cvSetMouseCallback("IDC_STATIC_OUTPUT", &mouse_move, this);
	// TODO: Add extra initialization here
	//img.Testing();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMaitriseImageRecDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMaitriseImageRecDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);

		if (kinectPic != NULL) {
			test.SetBitmap(kinectPic);
		}


		


	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMaitriseImageRecDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMaitriseImageRecDlg::Test(IKinectSensor * kinectSensor)
{
	// Initialize the Kinect and get coordinate mapper and the body reader
	IMultiSourceFrame* pMultiFrameSource = NULL;
	HRESULT hr;
	ICoordinateMapper* coordMap;




		hr = kinectSensor->get_CoordinateMapper(&coordMap);

	if (SUCCEEDED(hr))
	{
		DBOUT("YES");
	}
	else DBOUT("FUCK");


}

void CMaitriseImageRecDlg::drawVisualRect(cv::Rect& rect)
{
	DBOUT("Draw a rectangle (x,y,w,h) : " << rect.x << "," << rect.y << "," << rect.width << "," << rect.height << ")\n"); 
	cv::Mat temp = cvKinect.clone(); 
	cv::rectangle(temp, rect, cv::Scalar(0, 0, 200), 2, 8,0); 

	cv::imshow("IDC_STATIC_OUTPUT", temp);
	//cv::imshow("IDC_PIC_KINECT", temp); 
}

void CMaitriseImageRecDlg::OnNMThemeChangedImage(NMHDR *pNMHDR, LRESULT *pResult)
{
	// This feature requires Windows XP or greater.
	// The symbol _WIN32_WINNT must be >= 0x0501.
	// TODO: Add your control notification handler code here
	*pResult = 0;
}





void CMaitriseImageRecDlg::OnBnClickedOk()
{

	//img.Testing();


	
	HRESULT hr;
	
	CBitmap b;
	auto start = get_time::now(); //use auto keyword to minimize typing strokes :)
	auto end = get_time::now();

	hr = GetDefaultKinectSensor(&m_pKinectSensor);
	if (FAILED(hr))
	{
		return;
	}
	hr = m_pKinectSensor->Open();



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
		MSG msg;
		RGBQUAD *pBuffer = NULL;

		while (true)

		{
			while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{

				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
	
			if (!pauseCapture && WAIT_OBJECT_0 == WaitForSingleObject(reinterpret_cast<HANDLE>(m_MultiSourceEvent), INFINITE))
			{
				start = get_time::now();

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

					if (pBuffer == NULL || nWidth != width || nHeight != height) {
						if (pBuffer != NULL)
							delete pBuffer;
						pBuffer = new RGBQUAD[nWidth * nHeight];
						width = nWidth;
						height = nHeight;
						nBufferSize = nWidth * nHeight * sizeof(RGBQUAD);

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
							test.SetBitmap(b);
						}
						else if (imageFormat == ColorImageFormat_Yuy2) {
							BYTE* cc;
							
								nBufferSize = nWidth * nHeight * sizeof(RGBQUAD);

							//hr = colorFrame->CopyRawFrameDataToArray(nBufferSize, reinterpret_cast<BYTE*>(pBuffer));
							
							hr = colorFrame->CopyConvertedFrameDataToArray(nBufferSize, reinterpret_cast<BYTE*>(pBuffer), ColorImageFormat_Bgra);
							BYTE* testingTest = reinterpret_cast<BYTE*>(pBuffer);
							cvKinect = cv::Mat(nHeight, nWidth, CV_8UC4, reinterpret_cast<BYTE*>(pBuffer));
							//cv::imshow("", cvKinect);


							//CDC sdc;
							//sdc.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
							//CDC dc;
							//dc.CreateCompatibleDC(&sdc);
							//CBitmap bmp;
							//bmp.CreateCompatibleBitmap(&sdc, nWidth, nHeight);
							//bmp.SetBitmapBits(nBufferSize, testingTest);

						
							//kinectPic = (HBITMAP)bmp.Detach();

							imageLoaded = true;
							PostMessageA(this->m_hWnd, WM_FIND, (WPARAM)1, (LPARAM)2);

							end = get_time::now();
							std::ostringstream ss;
							auto time = end - start;
							//DBOUT("Time : " << ((chrono::duration_cast<ns>(time).count())/1000));
							//BYTE t = cc[4];
							//BYTE a = cc[0];

							

						
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

			if (WM_QUIT == msg.message)
			{
				break;
			}

		} // end main loop
		
	}
	
    
	
}



LRESULT  CMaitriseImageRecDlg::OnImageUpdated(WPARAM wParam, LPARAM lParam)
{
	//DBOUT("Ca marche");
	cvNamedWindow("IDC_STATIC_OUTPUT", 0);
	cvResizeWindow("IDC_STATIC_OUTPUT", 1920, 1080);
	CWnd* pic = GetDlgItem(IDC_PIC_KINECT);
	
	HWND hWnd = (HWND)cvGetWindowHandle("IDC_STATIC_OUTPUT");
	
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, pic->m_hWnd);

    cv::imshow("IDC_STATIC_OUTPUT", cvKinect);
	//test.SetBitmap(kinectPic);
	//::ShowWindow(hParent, SW_HIDE);

	return (LRESULT)1;

}


void CMaitriseImageRecDlg::OnBnClickedCancel()
{

}

BOOL CMaitriseImageRecDlg::PreTranslateMessage(MSG* pMsg) {
	int x = (int)pMsg->wParam; 
	HWND hWnd = (HWND)cvGetWindowHandle("IDC_STATIC_OUTPUT");
	std::string msgToGet = ""; 
	if (pMsg->message == WM_KEYDOWN)
	{
		if (x == VK_SPACE) {
			// SET THE SPACE ACTION
			DBOUT("Space is pressed"); 
			MessageBox(L"Space as been push", L"AreYouGood?", MB_OK);
		}

		if (x == VK_RETURN) {
			// Creates textbox for input
		
		}
	}
	return false;
}


/////////////////////////////////////////////////////////////////////////////////////
void mouse_move(int event, int x, int y, int flag, void* param) {
	CMaitriseImageRecDlg* cmird = (CMaitriseImageRecDlg*)param;
	int static pastX;
	int static pastY; 
	if (event == CV_EVENT_LBUTTONDOWN) {
		oldX = x;
		oldY = y; 
		DBOUT("MOUSE BUTTON DOWN\n");
		isDragged = true; 
	}
	else if (event == CV_EVENT_LBUTTONUP) {

		isDragged = false; 
		DBOUT("MOUSE BUTTON UP\n"); 
		if(zone.area() > 10)
		cmird->fileManager.saveImage("CategorieNo3", cmird->cvKinect(zone)); 
	}
	else if (!isDragged) {

	}
	else if (isDragged) {
		newX = x;
		newY = y;
		zone.x = (oldX < newX ? oldX : newX);
		zone.y = (oldY < newY ? oldY : newY);
		zone.height = (oldY < newY ? newY - oldY : oldY - newY);
		zone.width = (oldX < newX ? newX - oldX : oldX - newX);
		if (cmird->forceSquare) {
			zone.height = (zone.height > zone.width ? zone.height : zone.width);
			zone.width = (zone.width > zone.height ? zone.width : zone.height);
		}
		pastX = x; 
		pastY = y; 
		cmird->drawVisualRect(zone); 
	}

	if (false && true) {
		DBOUT("isDragged : " << isDragged << "oldX, oldY = " << oldX << "," << oldY << "newX, newY = " << newX << "," << newY << std::endl); 
		DBOUT("x,y is : " << x << " " << y << std::endl);
		DBOUT("Rect is (x,y,w,h): (" << zone.x << "," << zone.y << "," << zone.width << "," << zone.height << ")" << std::endl);
	}
}




void CMaitriseImageRecDlg::OnEnChangeCategorieEdit()
{
	//zone != NULL) {
	CEdit* editTitle = (CEdit*)GetDlgItem(IDC_CATEGORIE_EDIT);
	char* temp = new char[50];
	CString input;
	editTitle->GetWindowTextW(input);
	//editTitle->GetWindowText(temp, 50); 
	CT2CA converter(input);
	lastCategorie = std::string(converter);
	
}




void CMaitriseImageRecDlg::OnBnClickedCategorieSave()
{
	// DBOUT("From button : " << lastCategorie << "****************"); 
//	if(lastCategorie != NULL && !cvKinect.empty())
	fileManager.saveImage(lastCategorie, cvKinect(zone)); 
	// fileManager.saveImage("Bouteille", cvKinect(zone));
	// TODO: Add your control notification handler code here
}


void CMaitriseImageRecDlg::OnBnClickedShowFilter()
{

	// TODO: Add your control notification handler code here
}





void CMaitriseImageRecDlg::OnBnClickedPauseCapture()
{
	pauseCapture = true; 
	// TODO: Add your control notification handler code here
}


void CMaitriseImageRecDlg::OnBnClickedRestartCapture()
{
	pauseCapture = false; 
	// TODO: Add your control notification handler code here
}




void CMaitriseImageRecDlg::OnBnClickedForceSquare()
{
	
	CButton* checkButton = (CButton*)GetDlgItem(IDC_FORCE_SQUARE); 
	forceSquare = checkButton->GetCheck() == 1; 

	DBOUT("CheckButton forceSquare = " << (forceSquare ? "Set a true" : "Set a false") << std::endl); 

}
void CMaitriseImageRecDlg::OnBnClickedNeuralLocal()
{
	CButton* checkButton = (CButton*)GetDlgItem(IDC_NEURAL_LOCAL); 
	neuralLocal = checkButton->GetCheck() == 1; 
	DBOUT("CheckButton neuralLocal = " << (neuralLocal ? "Set a true" : "Set a false") << std::endl);
}


void CMaitriseImageRecDlg::OnBnClickedNeuralGlobal()
{
	CButton* checkButton = (CButton*)GetDlgItem(IDC_NEURAL_GLOBAL);
	neuralGlobal = checkButton->GetCheck() == 1; 
	DBOUT("CheckButton neuralGlobal = " << (neuralGlobal ? "Set a true" : "Set a false") << std::endl);
}

void CMaitriseImageRecDlg::OnBnClickedTestImageFixe()
{
	// TODO: Add your control notification handler code here
	cvNamedWindow("IDC_STATIC_OUTPUT", 0);

	cvKinect = cv::imread("res\\PerrierIn.png.png", cv::IMREAD_COLOR);
	cvResizeWindow("IDC_STATIC_OUTPUT", 1920, 1080);
	CWnd* pic = GetDlgItem(IDC_PIC_KINECT);

	HWND hWnd = (HWND)cvGetWindowHandle("IDC_STATIC_OUTPUT");

	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, pic->m_hWnd);


	IplImage tmp = cvKinect;
	//cvShowImage("IDC_STATIC_OUTPUT", &tmp);
	cv::imshow("IDC_STATIC_OUTPUT", cvKinect);

	//cvSetMouseCallback("IDC_STATIC_OUTPUT", &mouse_move, this);
	
	//CDialogEx::OnCancel();
	// TODO: Add your control notification handler code here
}



