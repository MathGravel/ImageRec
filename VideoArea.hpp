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

#include <opencv2/opencv.hpp>
#include "Kinect.h"


class VideoArea : public Gtk::DrawingArea
{
    protected:
    //cv::VideoCapture cv_cap;

    bool cv_opened;
        Kinect* kin;

    virtual bool on_draw (const Cairo::RefPtr<Cairo::Context> &cr);
        bool on_timeout ();
    public:
        VideoArea ();
        void StartKinect();
        void StopKinect();
        virtual ~VideoArea();
};

#endif	/* VIDEOAREA_HPP */

