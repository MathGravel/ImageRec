#include "VideoArea.hpp"



struct stat info;


VideoArea::VideoArea() : cv_opened(false) {

    sourceFeed = NULL;
    on_dragged = false;
    chosedROI = false;
    localRec = false;
    segmenting = false;
    act = ActivityRegion::instance();
    this->add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::BUTTON1_MOTION_MASK);
    Glib::signal_timeout().connect(sigc::mem_fun(*this, &VideoArea::on_timeout), 50);
    this->signal_button_press_event().connect(sigc::mem_fun(*this, &VideoArea::onMouseDown));
    this->signal_motion_notify_event().connect(sigc::mem_fun(*this, &VideoArea::onMouseMove));
    this->signal_button_release_event().connect(sigc::mem_fun(*this, &VideoArea::onMouseUp));
    //this->add_events(Gdk::DRAG_ENTER | Gdk::DRAG_LEAVE | Gdk::DRAG_MOTION);
}

VideoArea::~VideoArea() {
    //if (sourceFeed != NULL)
    //  delete sourceFeed;
}

bool VideoArea::onMouseDown(GdkEventButton *event) {
    on_dragged = true;
    chosenROI.release();
    chosenROI = NULL;
    if (event->button != 1) {
        rectROI = cv::Rect();
        on_dragged = false;
        chosedROI = false;
    } else {
        x1 = event->x;
        y1 = event->y;
    }


    return true;
}

bool VideoArea::onMouseUp(GdkEventButton *event) {
    std::cout << "OnMouseUp" << std::endl;
    if (on_dragged) {
        on_dragged = false;
    }
    return true;
}

bool VideoArea::onMouseMove(GdkEventMotion *event) {

    int newX, newY;
    if (on_dragged) {
        chosedROI = true;

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


void VideoArea::StartCamera(VideoSource *feed) {
    cv_opened = true;
    sourceFeed = feed;

}

void VideoArea::RestartCamera() {
    cv_opened = true;

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

template<typename Base, typename T>
inline bool instanceof(const T*) {
    return std::is_base_of<Base, T>::value;
}

bool VideoArea::on_draw(const Cairo::RefPtr<Cairo::Context> &cr) {

    // if (!cv_opened) return false;
    if (sourceFeed == NULL)
        return false;

    if (cv_opened) {

        sourceFeed->update();
        currentPic = sourceFeed->getColorFeed();
        currentDepthPic = sourceFeed->getDepthFeed();
        currentMappedPic = sourceFeed->getMappedFeed();
    }

    cv::Mat picShow;


    formattedPic.release();
    formattedPic = currentPic.clone();

    if (localRec) {

        act->Update(formattedPic,currentDepthPic);

        formattedPic = act->getImageWithROI();

    }

    if (chosedROI) {
        cv::rectangle(formattedPic, rectROI, cv::Scalar(0, 0, 200), 2, 8, 0);
        chosenROI = currentPic(rectROI);

    }

     cv::cvtColor(formattedPic, formattedPic, CV_BGR2RGB);


    Gdk::Cairo::set_source_pixbuf(cr,
                                  Gdk::Pixbuf::create_from_data(formattedPic.data, Gdk::COLORSPACE_RGB, false, 8,
                                                                formattedPic.cols,
                                                                formattedPic.rows, formattedPic.step));


    cr->paint();
    skipframes++;
    skipframes = skipframes % 10;
    //imgScreen.release();

    return true;
}

void VideoArea::SegmentClassifyROI() {
    if (!chosedROI)
        return;
    //regions.clear();
    //probs.clear();
    //regions.push_back(rectROI);

    Affordance roi = ActivityRegion::instance()->testManuallyROI(currentPic,rectROI);
    std::cout << roi << std::endl;


}




void VideoArea::SavePictures() {
    std::string imgName = "Can";
    cv::Mat currentPic = chosenROI.clone();

    cv::resize(currentPic,currentPic,Size(256,256));

    static int currVal = 0;
        std::string emp ;
        std::string empOrg;
        emp.append("/home/uqamportable/CLionProjects/ImageRec/cmake-build-debug/Images/Env/env.");
    empOrg.append("/home/uqamportable/CLionProjects/ImageRec/cmake-build-debug/Images/EnvOrg/env.");

    emp.append(std::to_string(currVal));
    empOrg.append(std::to_string(currVal));

    currVal++;
        empOrg = emp;
        empOrg.append(".org.png");
        emp.append(".png");
        cv::imwrite(emp,currentPic);
        cv::imwrite(empOrg,chosenROI);

    currentPic.release();

}

void VideoArea::SaveROI(const std::string fileLoc, const std::string itemClass) {

    std::string location = fileLoc;
    location.append("/");
    location.append(itemClass);
    std::string locOriginal = location;
    locOriginal.append("-org");
    std::string locRes = location;
    locRes.append("-res");


    cv::Mat resized;

    if (chosedROI) {
        if (stat(location.c_str(), &info) != 0) {
            const int dir_err = mkdir(location.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        }
        if (stat(locOriginal.c_str(), &info) != 0) {
            const int dir_err = mkdir(locOriginal.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        }
        if (stat(locRes.c_str(), &info) != 0) {
            const int dir_err = mkdir(locRes.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        }

        location.append("/");
        locOriginal.append("/");
        locRes.append("/");

        std::string filename = "";
        time_t t = time(0);   // get time now
        struct tm *now = localtime(&t);
        filename.append(std::to_string(now->tm_year + 1900) + '-');
        filename.append(std::to_string((now->tm_mon + 1)) + '-');
        filename.append(std::to_string((now->tm_mday)) + '-');
        filename.append(std::to_string((now->tm_hour)) + '-');
        filename.append(std::to_string((now->tm_min)) + '-');
        filename.append(std::to_string((now->tm_sec)));
        filename.append(".png");
        location.append(filename);
        locOriginal.append(filename);
        locRes.append(filename);


        //RGB
        cv::Mat chosenROI_RGB;
        cv::cvtColor(chosenROI, chosenROI_RGB, CV_RGB2BGR);
        resized = chosenROI_RGB.clone();
        cv::resize(resized,resized,cv::Size(8,8));
        cv::Mat org = this->currentPic.clone();
        cv::cvtColor(org, org, CV_RGB2BGR);


        cv::imwrite(location, chosenROI_RGB);
        cv::imwrite(locOriginal, org);
        cv::imwrite(locRes, resized);

        org.release();
        resized.release();
        chosenROI_RGB.release();
        //cv::imwrite(location, chosenROI);

    }
}