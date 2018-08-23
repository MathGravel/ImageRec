#include "ImageTreatment.h"

ImageTreatment::ImageTreatment(std::map<std::string,std::string> stream) :videoSave(false),source(nullptr),showPercentage(true),showName(true),showzone(true) {
    this->deserialize(stream);
    screenSize = std::make_pair(640,480);
    originalSize = std::make_pair(640,480);
}

ImageTreatment::~ImageTreatment() {
    delete source;
}


void ImageTreatment::deserialize(std::map<std::string,std::string> stream)
{
    if(source != nullptr){
        delete source;
        source = nullptr;
    }
    if(stream["sourceType"] == "Vidéo locale") {
        this->source = new RealSenseVideo(stream["sourceChemin"],stream["sourceCheminProfondeur"]);
        originalSize = std::make_pair(640,480);
        screenSize = std::make_pair(640,480);
    }
    else if (stream["sourceType"] == "Intel RealSense (D435)") {
        this->source = new RealSense();
        originalSize = std::make_pair(1280,720);


    }else if (stream["sourceType"] == "Microsoft Kinect (Xbox One)") {
        this->source = new Kinect();

    } else {
        this->source = new WebCam();

    }
    if(stream.count("Screen Size") > 0) {
        std::string temp = stream.at("Screen Size");
        int pos = temp.find(" ");
        int width = std::atoi(temp.substr(0,pos).c_str());
        int height = std::atoi(temp.substr(pos+1).c_str());
        screenSize = std::make_pair(width,height);
    }
    showName = (stream.at("affichageNom") == "1");
    showzone = (stream.at("affichageZone") == "1");
    showPercentage = (stream.at("affichagePoucentage") == "1");



}


void ImageTreatment::update() {
    if (source == nullptr)
        return;
    source->update();
    this->colorPicture = source->getColorFeed();
    this->depthPicture = source->getDepthFeed();
    cv::resize(this->colorPicture,this->resizedPicture,cv::Size(screenSize.first,screenSize.second));
    this->colorFeed.push_back(this->colorPicture);
    this->depthFeed.push_back(this->depthPicture);
    timestamp = source->getTimeStamp();
    timeposition = source->getTimePosition();
}

void ImageTreatment::saveVideos() {
    cv::VideoWriter col,dep,prog;
    col.open("trace/traceCouleur.mkv",cv::VideoWriter::fourcc('H','2','6','4'),30,cv::Size(originalSize.first,originalSize.second));
    dep.open("trace/traceProfondeur.mkv",cv::VideoWriter::fourcc('H','2','6','4'),30,cv::Size(originalSize.first,originalSize.second));
    prog.open("trace/traceProgramme.mkv",cv::VideoWriter::fourcc('H','2','6','4'),30,cv::Size(screenSize.first,screenSize.second));
    for (const auto color : this->colorFeed) {
        col << color;
    }
    for (const auto depth: this->depthFeed)
        dep << depth;
    for (const auto pr : this->programFeed)
        prog << pr;

    col.release();
    dep.release();
    prog.release();

}


cv::Mat ImageTreatment::getOriginalImage() {return this->colorPicture;}
cv::Mat ImageTreatment::getCurrentImage(){return this->resizedPicture;}
cv::Mat ImageTreatment::getDepthImage(){return this->depthPicture;}


void ImageTreatment::treatPicture(ActivityRegion *act) {
    int fontface = cv::FONT_HERSHEY_SIMPLEX;
    double scale = 1;
    int thickness = 1;
    int baseline = 0;
    cv::Mat pic = this->colorPicture.clone();

    if (!act->regions.empty()) {
        for (auto &reg : act->regions) {
            cv::rectangle(pic, reg, cv::Scalar(100, 100, 100), 3);
        }
    } else if (!act->items.empty()) {
        for (auto &reg :  act->items) {
            cv::rectangle(pic, reg.getObjPos(), cv::Scalar(0, 0, 250), 4);
            if (act->oldName == reg.getObjName()) {
                if (showzone)
                    cv::rectangle(pic, reg.getObjPos(), cv::Scalar(250, 0, 0), 4);
                std::string val = showName?  reg.getObjName() + " "  : "";
                val += showPercentage ?  std::to_string((int) floor(reg.getProb() * 100)) + "%" : "";

                cv::Size text = cv::getTextSize(val, fontface, scale, thickness, &baseline);
                cv::Rect textBox(reg.getObjPos());
                textBox.y += textBox.height;
                textBox.height = text.height;
                cv::putText(pic, val, cv::Point(textBox.x, textBox.y + text.height), fontface, scale, CV_RGB(0, 250, 0),
                    thickness, 8);
            }
        }
    }

    int i  = act->hands.getObjects().size();
    for (auto & hand :  act->hands) {
        if (showzone)
            cv::rectangle(pic, hand.getObjPos(), cv::Scalar(125, 125, 0), 4);

        std::string val = showName?  hand.getObjName() + " "  : "";
        val += showPercentage ?  std::to_string((int) floor(hand.getProb() * 100)) + "%" : "";

        cv::Size text = cv::getTextSize(val, fontface, scale, thickness, &baseline);
        cv::Rect textBox(hand.getObjPos());
        textBox.y += textBox.height;
        textBox.height = text.height;
        cv::putText(pic, val, cv::Point(textBox.x, textBox.y + text.height), fontface, scale, CV_RGB(0, 250, 0),
            thickness, 8);

    }

    if (! act->currentAffordance.empty()) {
        for (std::vector<AffordanceTime *>::const_iterator it =  act->currentAffordance.begin(); it !=  act->currentAffordance.end();it++) {

            Affordance &pos = (*it)->getAffordance();
            if (showzone)
                cv::rectangle(pic, pos.getRegion(), cv::Scalar(250, 0, 0), 4);

            std::string val = showName?  pos.getName() + " "  : "";
            val += showPercentage ?  std::to_string((int) floor(pos.getObjectProbability() * 100)) + "%" : "";

            cv::Size text = cv::getTextSize(val, fontface, scale, thickness, &baseline);
            cv::Rect textBox(pos.getRegion());
            textBox.y += textBox.height;
            textBox.height = text.height;
            cv::putText(pic, val, cv::Point(textBox.x, textBox.y + text.height), fontface, scale, CV_RGB(0, 250, 0),
                thickness, 8);
        }

    }
    this->imageTreated = pic;
    cv::resize(this->imageTreated,this->resizedPicture,cv::Size(screenSize.first,screenSize.second));
    programFeed.push_back(imageTreated);
    //cv::imwrite("tr.png",imageTreated);
}

