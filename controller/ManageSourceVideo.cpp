#include "ManageSourceVideo.h"

ManageSourceVideo::ManageSourceVideo(map<string,string> setting) :videoSave(false),source(nullptr),showPercentage(true),showName(true),showzone(true)/*,pastAffordance(NULL)*/
{
    chooseInputVideo(setting["Input_Video"],stoi(setting["Webcam_Width"]),stoi(setting["Webcam_Height"]),setting["pathSourceVideo"],setting["pathSourceVideoDepth"]);
}

ManageSourceVideo::~ManageSourceVideo() {
    delete source;
}

// width and height only use if it's a webcam cause resolution depend of the webcam
void ManageSourceVideo::chooseInputVideo(string typeInputVideo,int width,int height,string pathSourceVideo,string pathSourceVideoDepth){
	if(source != nullptr){
        delete source;
        source = nullptr;
    }
    if(typeInputVideo == "LocalVideo") {
        this->source = new RealSenseVideo(pathSourceVideo,pathSourceVideoDepth);
        originalSize = this->source->getScreenSize();
        screenSize = std::make_pair(1280,720);
    }
    else if (typeInputVideo == "RealSense") {
        this->source = new RealSense();
        originalSize = std::make_pair(1280,720);
        screenSize = std::make_pair(1280,720);
    }
    else if (typeInputVideo == "RealSenseBag") {
        this->source = new RealSenseBag(pathSourceVideo);
        originalSize = std::make_pair(1280,720);
        screenSize = std::make_pair(1280,720);
    }
    else if (typeInputVideo == "Kinect") {
        //this->source = new Kinect();
        cout<<"Kinect don't work (Not implemented yet) | TODO change chooseInputVideo in controller/ImageTreatment.cpp and then controller/source/Kinect \n";

    }else if(typeInputVideo == "Webcam") {
        this->source = new WebCam();
        originalSize = std::make_pair(width,height);
        screenSize = std::make_pair(width,height);
    } 
    else{
        cout<<"Input specified don't match with anyone\n RealSense chosen\n";
        this->source = new RealSense();
        originalSize = std::make_pair(1280,720);
        screenSize = std::make_pair(1280,720);
    }

}

bool ManageSourceVideo::hasNextFrame() {
	if (source != nullptr)
		return (source->getTimeStamp() != "-1");
	else
		return false;
}

void ManageSourceVideo::update() {
    if (source == nullptr)
        return;
    source->update();
    this->colorPicture = source->getColorFeed();
    this->depthPicture = source->getDepthFeed();
    cv::resize(this->colorPicture,this->resizedPicture,cv::Size(screenSize.first,screenSize.second));

    timestamp = source->getTimeStamp();
    timeposition = source->getTimePosition();
    
    notifyObservers();
}

//Not use
void ManageSourceVideo::saveVideos() {
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


cv::Mat ManageSourceVideo::getOriginalImage() {return this->colorPicture;}
cv::Mat ManageSourceVideo::getCurrentImage(){return this->resizedPicture;}
cv::Mat ManageSourceVideo::getDepthImage(){return this->depthPicture;}

void ManageSourceVideo::registerObserver(Observer *observer) {
    observers.push_back(observer);
}

void ManageSourceVideo::removeObserver(Observer *observer) {
    // find the observer
    auto iterator = std::find(observers.begin(), observers.end(), observer);

    if (iterator != observers.end()) { // observer found
        observers.erase(iterator); // remove the observer
    }
}

void ManageSourceVideo::notifyObservers() {
    for (Observer *observer : observers) { // notify all observers
        observer->updateByObservee(this->getOriginalImage(),this->getDepthImage());
    }
}
