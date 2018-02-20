#include "VideoArea.hpp"

#include <gdkmm/general.h>
#include <glibmm/main.h>
#include <gdkmm/pixbuf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <chrono>
#include <ctime>
#include <functional>     // std::ref


struct stat info;


VideoArea::VideoArea() : cv_opened(false) {

    sourceFeed = NULL;
    on_dragged = false;
    chosedROI = false;
    localRec = false;
    segmenting = false;
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


std::vector<cv::Rect> segmentPic(cv::Mat picture) {

    cv::Mat current = picture;
    cv::Mat currentSmall = current.clone();

    int height = current.rows;
    int width = current.cols;
    int newHeight = height;
    int newWidth = width;
    double ratiox = 1;
    double ratioy = 1;
    while (newHeight > 256 && newWidth > 200) {
        newHeight = newHeight / 1.25;
        ratiox = ratiox * 1.25;
        newWidth = newWidth / 1.25;
    }


    resize(current, currentSmall, cv::Size(newWidth, newHeight));


    auto regions = ss::selectiveSearch(currentSmall, 200, 0.9, 60, 400, 1000, 4);

    // do something...
    std::vector<cv::Rect> resizedRegions;
    for (auto &&rect : regions) {

        cv::Rect x(rect.x * ratiox, rect.y * ratiox, rect.width * ratiox, rect.height * ratiox);
        resizedRegions.push_back(x);


    }


    return resizedRegions;

}


bool VideoArea::on_draw(const Cairo::RefPtr<Cairo::Context> &cr) {

    // if (!cv_opened) return false;
    if (sourceFeed == NULL)
        return false;

    if (cv_opened) {

        sourceFeed->update();
        currentPic = sourceFeed->getColorFeed();
        cv::Mat mappedFeed = sourceFeed->getMappedFeed();
        cv::cvtColor(currentPic, currentPic, CV_BGR2RGB);


    }

    cv::Mat picShow;


    formattedPic.release();
    formattedPic = this->currentPic.clone();

    if (localRec) {
        if (!segmenting) {
            resultSeg = std::async(std::launch::async, segmentPic, currentPic.clone());
            segmenting = true;

        } else {
            if (resultSeg.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                regions.clear();
                probs.clear();
                regions = resultSeg.get();
                this->classifyPic(formattedPic);
                segmenting = false;
            }
        }


        std::vector<cv::Rect>::iterator it = regions.begin();
        std::vector<std::string>::iterator it2 = probs.begin();

        while (it != regions.end() && it2 != probs.end()) {
            cv::Rect reg = *it;
            std::string prob = *it2;
            cv::rectangle(formattedPic, reg, cv::Scalar(0, 255, 0), 3, 8);
            cv::putText(formattedPic, prob, cv::Point(reg.x, reg.y + reg.height), FONT_HERSHEY_SIMPLEX, 0.5,
                        Scalar(0, 0, 0), 4);

            it++;
            it2++;
        }


    }


    //cv::Mat imgScreen = formattedPic.clone();
    if (chosedROI) {
        cv::rectangle(formattedPic, rectROI, cv::Scalar(0, 0, 200), 2, 8, 0);
        chosenROI = currentPic(rectROI);

    }


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


cv::Mat VideoArea::AddData() {


    cv::Mat current = this->currentPic.clone();
    cv::Mat currentSmall = this->currentPic.clone();

    if (localRec) {


        int height = current.rows;
        int width = current.cols;
        int newHeight = height;
        int newWidth = width;
        double ratiox = 1;
        double ratioy = 1;
        while (newHeight > 256 && newWidth > 200) {
            newHeight = newHeight / 1.25;
            ratiox = ratiox * 1.25;
            newWidth = newWidth / 1.25;
        }
        // ratiox = ceil(ratiox);


        // int newWidth = width * newHeight / height;
        //ratioy = current.rows / newHeight;

        resize(current, currentSmall, cv::Size(newWidth, newHeight));

        // selective search
        if (skipframes == 0)
            regions = ss::selectiveSearch(currentSmall, 200, 0.9, 60, 400, 1000, 4);

        // do something...

        for (auto &&rect : regions) {
            cv::rectangle(currentSmall, rect, cv::Scalar(0, 255, 0), 3, 8);

            cv::Rect x(rect.x * ratiox, rect.y * ratiox, rect.width * ratiox, rect.height * ratiox);
            //rectangle(imO, x, Scalar(0, 255, 0));
            cv::rectangle(current, x, cv::Scalar(0, 255, 0), 3, 8);

        }


    }
    currentSmall.release();
    return current;
}


cv::Mat VideoArea::FindRegionProposals(cv::Mat picToSeg) {

}


void VideoArea::SegmentClassifyROI() {
    if (!chosedROI)
        return;
    regions.clear();
    probs.clear();
    regions.push_back(rectROI);
    probs.push_back(caffe.predict(chosenROI));
    std::cout << caffe.predict(chosenROI) << std::endl;

}


void VideoArea::classifyPic(cv::Mat &currentPic) {

    // if (classe == "")
    //    classe = caffe.predict(currentPic.clone());
    // cv::putText(currentPic,classe,cv::Point(rectROI.x,rectROI.y),8,2,cv::Scalar(100,100,100));
    //Socket::Send_Data(classe.c_str(),80);
    //std::cout << "\n DERP  ----" << classe << " ----- \n";
    probs.clear();
    for (auto &&rect : regions) {
        cv::Mat reg = currentPic(rect);
        probs.push_back(caffe.predict(reg));
        reg.release();
    }

}


void VideoArea::SaveROI(const std::string fileLoc, const std::string itemClass) {

    std::string location = fileLoc;
    location.append("/");
    location.append(itemClass);

    if (chosedROI) {
        if (stat(location.c_str(), &info) != 0) {
            const int dir_err = mkdir(location.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        }
        location.append("/");
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

        //RGB
        cv::Mat chosenROI_RGB;
        cv::cvtColor(chosenROI, chosenROI_RGB, CV_RGB2BGR);

        cv::imwrite(location, chosenROI_RGB);
        //cv::imwrite(location, chosenROI);

    }
}