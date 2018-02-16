#include "VideoArea.hpp"

#include <gdkmm/general.h>
#include <glibmm/main.h>
#include <gdkmm/pixbuf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <chrono>
#include <ctime>

struct stat info;


VideoArea::VideoArea() : cv_opened(false) {

    sourceFeed = NULL;
    on_dragged = false;
    chosedROI =false;
    this->add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::BUTTON1_MOTION_MASK);
    Glib::signal_timeout().connect(sigc::mem_fun(*this, &VideoArea::on_timeout), 50);
    this->signal_button_press_event().connect(sigc::mem_fun(*this,&VideoArea::onMouseDown));
    this->signal_motion_notify_event().connect(sigc::mem_fun(*this,&VideoArea::onMouseMove));
    this->signal_button_release_event().connect(sigc::mem_fun(*this,&VideoArea::onMouseUp));
    //this->add_events(Gdk::DRAG_ENTER | Gdk::DRAG_LEAVE | Gdk::DRAG_MOTION);
}

VideoArea::~VideoArea() {
    //if (sourceFeed != NULL)
      //  delete sourceFeed;
}

bool VideoArea::onMouseDown(GdkEventButton * event) {
    on_dragged = true;
    chosenROI.release();
    chosenROI = NULL;
    if (event->button != 1) {
        rectROI = cv::Rect();
        on_dragged = false;
        chosedROI = false;
    }
    else {
        x1 = event->x;
        y1 = event->y;
    }


    return true;
}

bool VideoArea::onMouseUp(GdkEventButton* event) {
    std::cout << "OnMouseUp" << std::endl;
    if (on_dragged) {
        on_dragged = false;
    }
    return true;
}

bool VideoArea::onMouseMove(GdkEventMotion *event) {


    int newX,newY;
    if (on_dragged) {
        std::cout << "OnMouseMoveInterieur" << std::endl;
        chosedROI = true;
    std::cout << "DERP" << std::endl;

    newX = event->x;
    newY = event->y;
    rectROI.x = (x1 < newX ? x1 : newX);
    rectROI.y = (y1 < newY ? y1 : newY);
    rectROI.height = (y1 < newY ? newY - y1 : y1 - newY);
    rectROI.width = (x1 < newX ? newX - x1 : x1 - newX);
    //if (cmird->forceSquare) {
      //  zone.height = (zone.height > zone.width ? zone.height : zone.width);
       // zone.width = (zone.width > zone.height ? zone.width : zone.height);
    //}
    }
    return true;
}


void VideoArea::StartCamera(VideoSource * feed) {
        cv_opened = true;
        sourceFeed = feed;

}

void VideoArea::StopCamera() {
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
    if (sourceFeed == NULL || !cv_opened)
        return false;

    sourceFeed->update();
    currentPic = sourceFeed->getColorFeed();
    cv::Mat mappedFeed = sourceFeed->getMappedFeed();
    cv::cvtColor(currentPic, currentPic, CV_BGR2RGB);

    if (segImg)
        currentPic = imgSeg.segmentPic(currentPic);

    cv::Mat picShow;
    if (chosedROI) {
        picShow = currentPic.clone();
        cv::rectangle(picShow,rectROI,cv::Scalar(0, 0, 200), 2, 8,0);
        chosenROI = currentPic(rectROI);
    } else
        picShow = currentPic;

    //auto test = selection::selectiveSearch(picShow,mappedFeed);

    //for (auto &e : test) {
      //  cv::rectangle(picShow,e.tl(),e.br(),cv::Scalar(0,200,0),2,8,0);
    //}

    Gdk::Cairo::set_source_pixbuf(cr, Gdk::Pixbuf::create_from_data(picShow.data, Gdk::COLORSPACE_RGB, false, 8, picShow.cols,
                                                                    picShow.rows, picShow.step));
    cr->paint();

    return true;
}

void  VideoArea::SaveROI(const std::string fileLoc, const std::string itemClass) {

    std::string location = fileLoc;
    location.append("/");
    location.append(itemClass);

    if (chosedROI) {
        if( stat(location.c_str() , &info ) != 0 ) {
            const int dir_err = mkdir(location.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        }
        location.append("/");
        std::string filename = "";
        time_t t = time(0);   // get time now
        struct tm * now = localtime( & t );
        filename.append(std::to_string(now->tm_year + 1900) + '-');
        filename.append(std::to_string((now->tm_mon + 1)) + '-');
        filename.append(std::to_string((now->tm_mday)) + '-');
        filename.append(std::to_string((now->tm_hour)) + '-');
        filename.append(std::to_string((now->tm_min)) + '-');
        filename.append(std::to_string((now->tm_sec)));
        filename.append(".png");
        location.append(filename);

        cv::imwrite(location,chosenROI);

    }
}