#include "YoloCPU.h"


YoloCPU::YoloCPU(float _prob) {

    resizeRatio = width / (float) height;

#ifdef USE_KITCHEN
    network = "ressources/models/yolov3-kitchen.backup";
    networkDef = "ressources/models/yolov3-kitchen.cfg";
    width = 640;
    height = 480;
#else
    network = "ressources/models/balls.backup";
    networkDef = "ressources/models/balls.cfg";
    width = 1280;
    height = 720;
#endif

    confidenceThreshold = _prob;
    neuralNetwork = cv::dnn::readNetFromDarknet(networkDef,network);


    if (width/ (float)height > resizeRatio)
    {
        cropSize = Size(static_cast<int>(height * resizeRatio),
                        height);
    }
    else
    {
        cropSize = Size(width,
                        static_cast<int>(width / resizeRatio));
    }
    crop = Rect(Point((width-cropSize.width) / 2, (height - cropSize.height) / 2),cropSize);


    startingPos = Point( (width- cropSize.width) / 2, (height - cropSize.height) / 2);

#ifdef USE_KITCHEN
    QFile names(":/models/c-kitchen.name");
#else
    QFile names(":/models/balls.name");
#endif
    names.open(QIODevice::ReadOnly); //| QIODevice::Text)
    QTextStream in(&names);
        int line = in.readLine().toInt();
        classNames = new std::string[line];
        line = 0;
    while( !in.atEnd())
    {
        classNames[line++]=in.readLine().toUtf8().constData();
    }
    names.close();

}

YoloCPU::~YoloCPU() {
    delete []classNames;
}

inline std::vector<DetectedObject> YoloCPU::findObjects(cv::Mat color,cv::Mat depth) {

    std::vector<DetectedObject> objets;



    Mat blob;


    blob = blobFromImage(color, 1.0f/250.0f,
                             Size(288, 288),Scalar(),true,false); //Convert Mat to batch of images


    neuralNetwork.setInput(blob);

    //neuralNetwork.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);

    std::vector<Mat> outs;
    std::vector<cv::String> outNames(2);

#ifdef USE_KITCHEN
        outNames[0] = "yolo_90";
        outNames[1] = "yolo_102";
#else
        outNames[0] = "yolo_90";
        outNames[1] = "yolo_102";
#endif
        //outNames[2] = "yolo_106";
        neuralNetwork.forward(outs, getOutputsNames(neuralNetwork));
            //neuralNetwork.forward(outs, outNames);

            std::vector<int> classIds;
            std::vector<float> confidences;
            std::vector<Rect2d> boxes;
            for (int i = 0; i < outs.size(); ++i)
            {
                Mat& out = outs[i];
                for (int j = 0; j < out.rows; ++j)
                {
                    Mat scores = out.row(j).colRange(5, out.cols);
                    double confidence;
                    Point maxLoc;
                    minMaxLoc(scores, 0, &confidence, 0, &maxLoc);
                    if (confidence < confidenceThreshold)
                        continue;
                    float* detection = out.ptr<float>(j);
                    double centerX = detection[0];
                    double centerY = detection[1];
                    double w = detection[2];
                    double h = detection[3];
                    int left  = (centerX-w/2.)*width;
                    int right = (centerX+w/2.)*width;
                    int top   = (centerY-h/2.)*height;
                    int bot   = (centerY+h/2.)*height;
                    cv::Rect obj(0,0,0,0);

                    if(left < 0) left = 0;
                    if(right > width-1) right = width-1;
                    if(top < 0) top = 0;
                    if(bot > height-1) bot = height-1;
                    obj.x = left;
                    obj.y = top;
                    obj.width = right - left;
                    obj.height = bot - top;

                    boxes.push_back(Rect2d(centerX - 0.5 * w, centerY - 0.5 * h,
                                           w, h));
                    confidences.push_back(confidence);
                    classIds.push_back(maxLoc.x);
                    cv::Rect2d central;
                    central.x = obj.x + obj.width/4;
                    central.y = obj.y + obj.height/4;
                    central.width = obj.width/2;
                    central.height = obj.height/2;

                    Scalar m = mean(depth(central));

                    //object.x += startingPos.x;
                    // object.y += startingPos.y;

                    std::string nom = classNames[maxLoc.x];
                    int offset = maxLoc.x*123457 % 10;
                    float red = get_color(2,offset,10);
                     float green = get_color(1,offset,10);
                     float blue = get_color(0,offset,10);
                      float rgb[3];
                      rgb[0] = red;
                      rgb[1] = green;
                      rgb[2] = blue;
                    DetectedObject obbj(obj,nom,m[0],confidence,red,green,blue);

                    objets.push_back(obbj);
                }
            }

            std::vector<bool> posi(objets.size());
            std::fill(posi.begin(),posi.end(),true);

            std::vector<DetectedObject> objs;
            for (int i = 0; i < objets.size();i++) {
                DetectedObject obj(objets[i]);
                if (posi[i]) {
                    for (int j = i + 1; j < objets.size();j++) {
                        if (obj.getObjName() != objets[j].getObjName())
                            continue;
                        int ar = (obj.getObjPos() & objets[j].getObjPos()).area();
                        if ((ar * 1.3 > obj.getObjPos().area() || ar * 1.3 > objets[j].getObjPos().area()) &&
                                obj.getObjName() == objets[j].getObjName()) {
                            posi[i] = false;
                            posi[j] = false;
                            obj.fusePosition(objets[j]);
                        }
                    }
                    objs.push_back(obj);
                }

            }




    return objs;


}

std::vector<String> YoloCPU::getOutputsNames(const Net& net)
{
    static std::vector<String> names;
    if (names.empty())
    {
        std::vector<int> outLayers = net.getUnconnectedOutLayers();
        std::vector<String> layersNames = net.getLayerNames();
        names.resize(outLayers.size());
        for (size_t i = 0; i < outLayers.size(); ++i)
            names[i] = layersNames[outLayers[i] - 1];
    }
    return names;
}

float YoloCPU::get_color(int c, int x, int max)
{
    float ratio = ((float)x/max)*5;
    int i = floor(ratio);
    int j = ceil(ratio);
    ratio -= i;
    float r = (1-ratio) * colors[i][c] + ratio*colors[j][c];
    //printf("%f\n", r);
    return r;
}
