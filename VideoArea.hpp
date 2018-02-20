/* 
 * File:   VideoArea.hpp
 * Author: imam
 *
 * Created on 3 October 2015, 1:03 AM
 */

#ifndef VIDEOAREA_HPP
#define	VIDEOAREA_HPP

#include <gtkmm/drawingarea.h>
#include <cairomm/context.h>
#include "selectivesearch.hpp"
#include <opencv2/opencv.hpp>
#include "Kinect.h"
#include "ImageSegmentationManual.h"
#include <thread>         // std::thread
#include <future>         // std::promise, std::future
#include "Header files/CNN/CaffeCNN.h"
#include "Socket.h"


class VideoArea : public Gtk::DrawingArea
{

public:
    VideoArea ();
    void StartCamera(VideoSource * feed = NULL);
    void RestartCamera();
    void SegmentClassifyROI();

    void StopCamera();
    virtual ~VideoArea();

    cv::Mat getChosenRoi() {
        return chosenROI;
    }

    bool hasChosenROI(){
        return chosedROI;
    }

    void setLocalSegmentation() {
        localRec = !localRec;
    }

    void setSegImage() {segImg = !segImg;}
    void  SaveROI(const std::string fileLoc, const std::string itemClass);
    std::string classe = "";
    bool globalRec = false;
protected:
    bool cv_opened;
    VideoSource * sourceFeed;


    virtual bool on_draw (const Cairo::RefPtr<Cairo::Context> &cr);
    bool onMouseDown(GdkEventButton * event);
    bool onMouseMove(GdkEventMotion * event);
    bool onMouseUp(GdkEventButton * event);
    bool on_timeout ();
    bool segImg = false;
    ImageSegmentationManual imgSeg;


private:
    bool on_dragged;
    bool chosedROI;
    bool localRec;
    bool segmenting = false;
    int skipframes = 0;
    int x1,x2,y1,y2;
    cv::Rect rectROI;
    CaffeCNN caffe;
    cv::Mat chosenROI;
    cv::Mat currentPic;
    cv::Mat formattedPic;
    cv::Mat AddData();


    std::vector<cv::Rect> regions;
    std::vector<std::string> probs;

    void classifyPic(cv::Mat& currentPic);
    cv::Mat FindRegionProposals(cv::Mat picToSeg);
    std::future<std::vector<cv::Rect>> resultSeg;

};

#endif	/* VIDEOAREA_HPP */

