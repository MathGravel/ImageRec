#include "YoloCPU.h"


YoloCPU::YoloCPU(float _prob) {

    width = 1280;
    height = 720;

    resizeRatio = width / (float) height;
    network = "ressources/models/yolov3.backup";
    confidenceThreshold = _prob;
    networkDef = "ressources/models/yolov3.cfg";
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
    //crop.width = crop.width + crop.x >= imgWidth ? crop.width - crop.x: crop.width;
    //crop.height = crop.height + crop.y >= imgHeight ? crop.height - crop.y: crop.height;

    startingPos = Point( (width- cropSize.width) / 2, (height - cropSize.height) / 2);

    std::ifstream inputFile( "/home/baptiste/Documents/reconnaissance-plans-activites/src/ressources/models/classes.name");        // Input file stream object

    // Check if exists and then open the file.
    if (inputFile.good()) {
        // Push items into a vector
        int nbClasses = 0;
        inputFile >> nbClasses;
        classNames = new std::string[nbClasses + 1];
        int i = 0;
        std::string temp;
        while (inputFile >> temp){
            classNames[i] = temp;
            i++;
        }
        // Close the file.
        inputFile.close();

    } else {
        std::cerr << "The classes names file is not available";
        exit(-1);
    }

}

YoloCPU::~YoloCPU() {
    delete classNames;
}

std::vector<DetectedObject> YoloCPU::findObjects(cv::Mat color,cv::Mat depth) {

    std::vector<DetectedObject> objets;



    Mat blob;

    //color = color(crop);
    //depth = depth(crop);


        blob = blobFromImage(color, 1.0f/200.0f,
                             Size(320, 320),Scalar(127.5,127.5,127.5),true,false); //Convert Mat to batch of images


    neuralNetwork.setInput(blob);
   // Mat detections = neuralNetwork.forward();
   // Mat matricesDet(detections.size[2],detections.size[3],CV_32F,detections.ptr<float>());


    std::vector<Mat> outs;
    std::vector<cv::String> outNames(2);
        outNames[0] = "yolo_82";
        outNames[1] = "yolo_94";
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
                    if (confidence < 0.3)
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
                    Scalar m = mean(depth(obj));

                    //object.x += startingPos.x;
                    // object.y += startingPos.y;

                    std::string nom = classNames[maxLoc.x-1];

                    DetectedObject obbj(obj,nom,m[0],confidence);
                    objets.push_back(obbj);
                }
            }


    return objets;


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
