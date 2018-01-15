
// MaitriseImageRecDlg.h : header file
//

#include "MaitriseImageRec.h"
#include "inkpicture1.h"
#include <thread>         // std::thread
#include <opencv2/opencv.hpp>

#include <mutex>          // std::mutex
#include <windows.h>
#include "FileManager.h"
#include "GoogleVisionCNN.h"
#include "ImageTreatment.h"
#include "NtKinect.h"

#include "CInputDialog.h"
//#include "resource.h"

#pragma once


// CMaitriseImageRecDlg dialog
class CMaitriseImageRecDlg : public CDialogEx
{
// Construction
public:
	CMaitriseImageRecDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAITRISEIMAGEREC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//CMaitriseImageRecApp img;
	

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnUpdatePicture(WPARAM wParam, LPARAM lParam);
	void OnDraw(CDC* pDC);
	void FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin);

	DECLARE_MESSAGE_MAP()
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
	WAITABLE_HANDLE m_MultiSourceEvent;
	bool imageLoaded = false;
	HBITMAP kinectPic = NULL;
	int width = -1;
	int height = -1;
	static void Test(IKinectSensor* kinectSensor);
	GoogleVisionCNN ccv;
	cv::Mat cvKinect;
	cv::Mat cvKinectFiltered;
	ImageTreatment imT;
	void CMaitriseImageRecDlg::create_rgbd(cv::Mat& depth_im, cv::Mat& rgb_im, cv::Mat& rgbd_im);
	NtKinect kinect;
	
public:
	FileManager fileManager;
	std::string lastCategorie =  ""; 
	bool pauseCapture = false; 
	bool forceSquare = false; 

	bool neuralLocal = false; 
	bool neuralGlobal = false; 

	afx_msg void OnNMThemeChangedImage(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnImageUpdated(WPARAM wParam, LPARAM lParam);
//	int ffgg;
//	CInkpicture1 Kinect;
	CStatic test;
	afx_msg void OnStnClickedPicKinect();
	afx_msg void OnBnClickedCancel();

	virtual BOOL PreTranslateMessage(MSG * pMsg);

	void drawVisualRect(cv::Rect& rect);

	afx_msg void OnEnChangeCategorieEdit();
	afx_msg void OnBnClickedCategorieSave();
	afx_msg void OnBnClickedShowFilter();

	afx_msg void OnBnClickedPauseCapture();
	afx_msg void OnBnClickedForceSquare();
	afx_msg void OnBnClickedTestImageFixe();
	cv::Mat TextInMat(cv::Mat mat, std::string content, cv::Rect rect);
	afx_msg void OnBnClickedNeuralLocal();
	afx_msg void OnBnClickedNeuralGlobal();
};
