
// MaitriseImageRecDlg.h : header file
//

#include "MaitriseImageRec.h"
#include "inkpicture1.h"
#include <thread>         // std::thread
#include <opencv2/opencv.hpp>

#include <mutex>          // std::mutex
#include <windows.h>
#include "FileManager.h"
#include "CInputDialog.h"

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

	
	
	
public:
	FileManager fileManager;
	cv::Mat cvKinect;
	std::string lastCategorie =  ""; 
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
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeCategorieEdit();
	afx_msg void OnBnClickedCategorieSave();
};
