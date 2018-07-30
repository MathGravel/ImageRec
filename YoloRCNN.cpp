//
// Created by uqamportable on 12/03/18.
//

#include "YoloRCNN.h"

YoloRCNN::YoloRCNN(std::string inference_path, int imgHeight,int imgWidth,bool estMain, float _prob) {

    width = 300;
    height = 300;
    resizeRatio = width / (float) height;
    network = inference_path + "/yolov2-ttt.backup";
    confidenceThreshold = _prob;
    networkDef = inference_path + "/yolov2-ttt.cfg";
    main = estMain;
    neuralNetwork = cv::dnn::readNetFromDarknet(networkDef,network);
    std::cout << "11111111111111" << std::endl;
    //neuralNetwork.setPreferableBackend()
    //cropSize = Size(static_cast<int>(imgHeight * resizeRatio),imgWidth);

    if (imgWidth/ (float)imgHeight > resizeRatio)
    {
        cropSize = Size(static_cast<int>(imgHeight * resizeRatio),
                        imgHeight);
    }
    else
    {
        cropSize = Size(imgWidth,
                        static_cast<int>(imgWidth / resizeRatio));
    }
    crop = Rect(Point((imgWidth-cropSize.width) / 2, (imgHeight - cropSize.height) / 2),cropSize);
    //crop.width = crop.width + crop.x >= imgWidth ? crop.width - crop.x: crop.width;
    //crop.height = crop.height + crop.y >= imgHeight ? crop.height - crop.y: crop.height;

    startingPos = Point( (imgWidth- cropSize.width) / 2, (imgHeight - cropSize.height) / 2);
}

YoloRCNN::~YoloRCNN() {

}


std::vector<DetectedObject> YoloRCNN::findObjects(cv::Mat color,cv::Mat depth) {

    std::vector<DetectedObject> objets;

    Mat blob = blobFromImage(color, 1/255.5,
                                  Size(640, 480),Scalar(127.5,127.5,127.5),true,false); //Convert Mat to batch of images

    neuralNetwork.setInput(blob);
    std::vector<Mat> outs;
    std::vector<cv::String> outNames(1, "detection_out");

    neuralNetwork.forward(outs, outNames);

    color = color(crop);
    depth = depth(crop);


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

            float* detection = out.ptr<float>(j);
            double centerX = detection[0];
            double centerY = detection[1];
            double width = detection[2];
            double height = detection[3];
            boxes.push_back(Rect2d(centerX - 0.5 * width, centerY - 0.5 * height,
                                   width, height));
            confidences.push_back(confidence);
            classIds.push_back(maxLoc.x);

            Rect object(Rect2d(centerX - 0.5 * width, centerY - 0.5 * height,
                               width, height));

            object = object  & cv::Rect(0, 0, depth.cols, depth.rows);


            Scalar m = mean(depth(object));
            object.x += startingPos.x;
            object.y += startingPos.y;
            DetectedObject obj(object,classNames[maxLoc.x],m[0],confidence);

        }
    }


    return objets;


}
//
// Created by uqamportable on 23/07/18.
//

