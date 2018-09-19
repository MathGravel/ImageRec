#include "ImageTreatment.h"

ImageTreatment::ImageTreatment() :videoSave(false),source(nullptr),showPercentage(true),showName(true),showzone(true),pastAffordance(NULL)
 {
    //this->deserialize(stream);
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
        originalSize = this->source->getScreenSize();
        screenSize = std::make_pair(1280,720);
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

}


void ImageTreatment::update() {
    if (source == nullptr)
        return;
    source->update();
    this->colorPicture = source->getColorFeed();
    this->depthPicture = source->getDepthFeed();
    cv::resize(this->colorPicture,this->resizedPicture,cv::Size(screenSize.first,screenSize.second));
    //cv::resize(this->depthPicture,this->depthPicture,cv::Size(screenSize.first,screenSize.second));

    //this->colorFeed.push_back(this->colorPicture.clone());
    //this->depthFeed.push_back(source->getOriginalDepth().clone());
    timestamp = source->getTimeStamp();
    timeposition = source->getTimePosition();
}

void ImageTreatment::saveVideos() {
    cv::VideoWriter col,dep,prog;
    col.open("traces/traceCouleur.mkv",cv::VideoWriter::fourcc('H','2','6','4'),30,cv::Size(originalSize.first,originalSize.second));
    dep.open("traces/traceProfondeur.mkv",cv::VideoWriter::fourcc('H','2','6','4'),30,cv::Size(originalSize.first,originalSize.second));
    prog.open("traces/traceProgramme.mkv",cv::VideoWriter::fourcc('H','2','6','4'),30,cv::Size(screenSize.first,screenSize.second));
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
    int thickness = 2;
    int baseline = 0;
    cv::Mat pic = this->colorPicture.clone();

    if (!act->regions.empty()) {
        for (auto &reg : act->regions) {
            cv::rectangle(pic, reg, cv::Scalar(100, 100, 100), 3);
        }
    } else if (!act->items.empty()) {
        for (auto &reg :  act->items) {

                if (showzone)
                    cv::rectangle(pic, reg.getObjPos(), cv::Scalar(reg.getRed(),reg.getGreen(),reg.getBlue()), 6);
                std::string val = showName?  reg.getObjName() + " "  : "";
                val += showPercentage ?  std::to_string((int) floor(reg.getProb() * 100)) + "%" : "";
                cv::Scalar color(reg.getRed(),reg.getGreen(),reg.getBlue());
                if (pastAffordance != NULL && pastAffordance->getName() == reg.getObjName()) {
                    color= cv::Scalar(255,255,255);
                }

                cv::Size text = cv::getTextSize(val, fontface, scale, thickness, &baseline);
                cv::Rect textBox(reg.getObjPos());
                textBox.y -= text.height;

                textBox.height = text.height;
                textBox.width = text.width * 1.04;
                cv::rectangle(pic,textBox,color,-1);

                cv::putText(pic, val, cv::Point(textBox.x, textBox.y + text.height), fontface, scale, CV_RGB(0, 0, 0),
                    thickness, 8);

        }
    }

    int i  = act->hands.getObjects().size();
    for (auto & hand :  act->hands) {
       // if (showzone)
            cv::rectangle(pic, hand.getObjPos(), cv::Scalar(hand.getRed(),hand.getGreen(),hand.getBlue()), 6);

        std::string val = showName?  hand.getObjName() + " "  : "";
        val += showPercentage ?  std::to_string((int) floor(hand.getProb() * 100)) + "%" : "";

        cv::Size text = cv::getTextSize(val, fontface, scale, thickness, &baseline);
        cv::Rect textBox(hand.getObjPos());
        textBox.y -= text.height;

        textBox.height = text.height;
        textBox.width = text.width * 1.04;
        cv::rectangle(pic,textBox,cv::Scalar(hand.getRed(),hand.getGreen(),hand.getBlue()),-1);

        cv::putText(pic, val, cv::Point(textBox.x, textBox.y + text.height), fontface, scale, CV_RGB(0, 0, 0),
            thickness, 8);

    }

    if (! act->currentAffordance.empty()) {
        for (std::vector<AffordanceTime *>::const_iterator it =  act->currentAffordance.begin(); it !=  act->currentAffordance.end();it++) {

            Affordance &pos = (*it)->getAffordance();
            pastAffordance = *it;
            if (showzone)
                cv::rectangle(pic, pos.getRegion(), cv::Scalar(250, 0, 0), 4);

            std::string val = showName?  pos.getName() + " "  : "";
            val += showPercentage ?  std::to_string((int) floor(pos.getObjectProbability() * 100)) + "%" : "";

            cv::Size text = cv::getTextSize(val, fontface, scale, thickness, &baseline);
            cv::Rect textBox(pos.getRegion());
            textBox.y -= text.height;

            textBox.height = text.height;
            textBox.width = text.width * 1.04;
            cv::rectangle(pic,textBox,cv::Scalar(255,255,255),-1);
            cv::putText(pic, val, cv::Point(textBox.x, textBox.y + text.height), fontface, scale, CV_RGB(0, 0, 0),
                thickness, 8);
        }

    }
    this->imageTreated = pic;
    cv::resize(this->imageTreated,this->resizedPicture,cv::Size(screenSize.first,screenSize.second));
    programFeed.push_back(resizedPicture.clone());
}

