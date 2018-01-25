#include "VideoArea.hpp"

#include <gdkmm/general.h>
#include <glibmm/main.h>
#include <gdkmm/pixbuf.h>


VideoArea::VideoArea() : cv_opened(false) {

    kin = NULL;

    //Use these if you need to work without the Kinect
    // cv_cap.open(0);

    // if (cv_cap.isOpened() == true) {
    //    cv_opened = true;
    //    Glib::signal_timeout().connect(sigc::mem_fun(*this, &VideoArea::on_timeout), 50);
    // }

    // kin = new Kinect;


}

VideoArea::~VideoArea() {

}

void VideoArea::StartKinect() {
    cv_opened = true;
    if (kin == NULL) {
        kin = new Kinect();
        Glib::signal_timeout().connect(sigc::mem_fun(*this, &VideoArea::on_timeout), 50);
    }
}

void VideoArea::StopKinect() {
    cv_opened = false;
}


bool VideoArea::on_timeout() {
    Glib::RefPtr<Gdk::Window> win = get_window();
    if (win) {
        Gdk::Rectangle r(0, 0, get_allocation().get_width(), get_allocation().get_height());
        win->invalidate_rect(r, false);
    }
    return true;
}

bool VideoArea::on_draw(const Cairo::RefPtr<Cairo::Context> &cr) {

    // if (!cv_opened) return false;
    if (kin == NULL || !cv_opened)
        return false;

    kin->update();
    cv::Mat pic = kin->getcurrentColorFrame();
    //cv::Mat cv_frame,cv_frame1;
    //cv_cap.read(cv_frame);

    // if (cv_frame.empty()) return false;

    cv::cvtColor(pic, pic, CV_BGR2RGB);


    Gdk::Cairo::set_source_pixbuf(cr, Gdk::Pixbuf::create_from_data(pic.data, Gdk::COLORSPACE_RGB, false, 8, pic.cols,
                                                                    pic.rows, pic.step));
    //Gdk::Cairo::set_source_pixbuf (cr, Gdk::Pixbuf::create_from_data(cv_frame1.data, Gdk::COLORSPACE_RGB, false, 8, cv_frame1.cols, cv_frame1.rows, cv_frame1.step));
    cr->paint();

    return true;
}