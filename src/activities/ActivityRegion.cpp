#include "ActivityRegion.h"

ActivityRegion* ActivityRegion::ar_instance = nullptr;

ActivityRegion::ActivityRegion():
                                 objectDetector(0.31f),
                                 currentlySegmenting(false),newRegions(false),newAffordance(false),oldName("") {
}


std::vector<cv::Rect> segmentPic(cv::Mat picture,cv::Mat depthPic);

void ActivityRegion::Update(cv::Mat vision,cv::Mat depthVision) {

    std::vector<cv::Rect> objects;
    currentImage = vision;
    currentImageDepth = depthVision;
    currentAffordance.clear();


        auto ii = this->detectObjets(vision, depthVision);

        //Test pour le corpus Kitchen ou les mains sont dans le meme systeme.
        std::vector<DetectedObject> mains;
        std::vector<DetectedObject> newit;

        for (const auto it : ii) {
            if (it.getObjName() == "hand" || it.getObjName() == "Hand") {
                mains.push_back(it);
                //exit(-1);
            }
            else {
                newit.push_back(it);
            }

        }
        mtx.lock();
        hands.clear();
        items.clear();
        hands = DetectedObjects(mains);
        items = DetectedObjects(newit);
        mtx.unlock();

        if (!items.empty() && !hands.empty()) {
            currentAffordance = affordances.findAffordances(items, hands);
            if (!currentAffordance.empty()) {

                for (auto it : currentAffordance)
                    currentAffordances.push(it);
            }
        }

}

void ActivityRegion::updateManualROI(cv::Mat vision, cv::Mat depthVision, cv::Rect chosenROI) {

    currentImage = vision;
    currentImageDepth = depthVision;
    hands.clear();
    items.clear();

    regions.push_back(chosenROI);

    hands = this->detectHand(vision,depthVision);

    //A completer.

}
/*
Affordance ActivityRegion::testManuallyROI(cv::Mat vision,  cv::Rect chosenROI) {
    std::pair<std::string,float> prediction = caffe.predict(vision(chosenROI)) ;
    Affordance aff(prediction.first,prediction.second,chosenROI,prediction.second);
    return aff;
}


std::vector<DetectedObject> ActivityRegion::confirmAffordance(const std::vector<cv::Rect>& objets, const cv::Mat & picture, const cv::Mat & depth) {

    std::vector<DetectedObject> classes;

    for (auto & region : objets) {
        for (auto& hand : hands) {
        if ( (region & hand.getObjPos()).area() > 0) {
            std::pair<std::string, float> prediction = caffe.predict(picture(region));
            DetectedObject detected(region, prediction.first, mean(depth(region))[0], prediction.second);
            classes.push_back(detected);
        }
        }
    }
    return classes;
}*/




std::vector<cv::Rect> segmentPic(cv::Mat picture,const cv::Mat depthPic) {

    cv::Mat currentSmall = picture.clone();
    cv::Mat currentDepthSmall = depthPic.clone();

    int height = picture.rows;
    int width = picture.cols;
    int newHeight = height;
    int newWidth = width;
    double ratiox = 1;
    double ratioy = 1;
    while (newHeight > 256 && newWidth > 200) {
        newHeight = newHeight / 1.25;
        ratiox = ratiox * 1.25;
        newWidth = newWidth / 1.25;
    }


    resize(picture, currentSmall, cv::Size(newWidth, newHeight));
    resize(depthPic, currentDepthSmall, cv::Size(newWidth, newHeight));

    auto regions = selectiveDepth::selectiveSearchDepth(currentSmall,currentDepthSmall, 150, 0.9, 30, 300, currentSmall.rows * currentSmall.cols/6, 50);

    std::vector<cv::Rect> newRect;
    cv::groupRectangles(regions,1,0.1);

    // do something...
    std::vector<cv::Rect> resizedRegions;
    for (auto &&rect : regions) {

        cv::Rect x(rect.x * ratiox, rect.y * ratiox, rect.width * ratiox, rect.height * ratiox);
        resizedRegions.push_back(x);
    }

    return resizedRegions;

}

DetectedObjects ActivityRegion::removeRedundantRegions(cv::Mat depth,DetectedObjects local,  std::list<DetectedObject> ssd, int confidence){
    std::vector<DetectedObject> nouv;
    DetectedObject* currentObj;
    double ratio = 1;
    for (auto  loc : local){
        nouv.push_back(loc);
        for (auto ss : ssd ) {
            double val = (1/9)* leveshDist(ss.getObjName(),loc.getObjName()) +
                    (2/9) * ss.getProb() + (2/9) * this->calcSimofDistDepth(ss.getObjPos(),loc.getObjPos(),depth) +
                    (4/9) * this->calcSimofDepth(this->calcDepthHist(depth,ss.getObjPos()),this->calcDepthHist(depth,loc.getObjPos()));
            ratio = std::min(ratio,val);
            currentObj = &ss;
        }
        if (ratio > confidence){
            nouv.push_back(*currentObj);
        }
        ratio = 1;
    }
    return DetectedObjects(nouv);
}

double ActivityRegion::leveshDist(const std::string& s1, const std::string& s2)
{
    int ln = s1.size();
    int ln2 = s2.size();
    std::vector<std::vector<unsigned int>> dist(ln + 1, std::vector<unsigned int>(ln2 + 1));

    dist[0][0] = 0;
    for(unsigned int i = 1; i <= ln; ++i) dist[i][0] = i;
    for(unsigned int j = 1; j <= ln2; ++j) dist[0][j] = j;

    for(unsigned int i = 1; i <= ln; ++i)
        for(unsigned int j = 1; j <= ln2; ++j) {
                    int check = (s1[i - 1] == s2[j - 1] ? 0 : 1);
                      dist[i][j] = std::min({ dist[i - 1][j] + 1, dist[i][j - 1] + 1, dist[i - 1][j - 1] + check });
        }
    double levDist = dist[ln][ln2] / std::max(s1.length(),s1.length());
    return levDist;
}

 std::vector<float> ActivityRegion::calcDepthHist( const cv::Mat &imgDepth,cv::Rect region )
{

    int channels[] = { 0 };
    const int bins = 25;
    int histSize[] = { bins };
    float range[] = { 0, 256 };
    const float *ranges[] = { range };

    std::vector<float> features;

        cv::Mat hist;

        cv::Mat input = imgDepth(region);

        cv::calcHist( &input, 1, channels, cv::Mat(), hist, 1, histSize, ranges, true, false );

        cv::normalize( hist, hist, 1.0, 0.0, cv::NORM_L1 );

        std::vector<float> histogram;
        hist.copyTo( histogram );

        if ( features.empty() )
        {
            features = std::move( histogram );
        }
        else
        {
            std::copy( histogram.begin(), histogram.end(), std::back_inserter( features ) );
        }


    return features;
}

 double ActivityRegion::calcSimofDepth( const std::vector<float> &r1, const std::vector<float> &r2  )
{
    assert( r1.size() == r2.size() );

    float sum = 0.0;

    for ( auto i1 = r1.cbegin(), i2 = r2.cbegin(); i1 != r1.cend(); i1++, i2++ )
    {
        sum += std::sqrt( *i1 * *i2 );
    }

    return sum;
}

 double ActivityRegion::calcSimofDistDepth( const cv::Rect &r1, const cv::Rect &r2, cv::Mat & depth )
{
    double minval, maxval;
    cv::minMaxLoc(depth,&minval,&maxval);
    double sum = cv::mean(depth(r1 & r2))[0] - cv::mean(depth(r1))[0] - cv::mean(depth(r2))[0];
    sum = sum / maxval;
    return sum;
}

template<typename Base, typename T>
inline bool instanceof(const T*) {
    return std::is_base_of<Base, T>::value;
}

DetectedObjects ActivityRegion::detectHand(cv::Mat color, cv::Mat depth) {
    return DetectedObjects(objectDetector.findObjects(color,depth)) ;
}

DetectedObjects ActivityRegion::detectObjets(cv::Mat color, cv::Mat depth) {
    return DetectedObjects(objectDetector.findObjects(color,depth)) ;
}

void ActivityRegion::reset() {
    currentAffordance.clear();
    this->affordances.clearCurrentAffordances();
    this->hands.clear();
    this->items.clear();
    this->regions.clear();
}
