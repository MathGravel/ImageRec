//
// Created by uqamportable on 12/03/18.
//

#include "MaskRCNN.h"

MaskRCNN::MaskRCNN(std::string inference_path, int imgHeight,int imgWidth,bool estMain) {

    width = 300;
    height = 300;
    resizeRatio = width / (float) height;
    meanRatio = 127;
    scaleFactor = 0.01f;
    network = inference_path + "/graph.pb";
    networkDef = inference_path + "/label.pbtxt";
    main = estMain;
    neuralNetwork = cv::dnn::readNetFromTensorflow(network,networkDef);
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

MaskRCNN::~MaskRCNN() {

}


std::vector<DetectedObject> MaskRCNN::findObjects(cv::Mat color,cv::Mat depth) {

    std::vector<DetectedObject> objets;
  //  this->color_pic.release();
    //this->depth_pic.release();
    //this->originalColor.release();
    //this->originalColor = color.clone();


    //this->color_pic = color.clone();
    //this->depth_pic = depth.clone();


    //cv::resize(this->color_pic,this->color_pic,Size(1280,720));

   // Mat blob = blobFromImage(this->color_pic,scaleFactor,Size(width,height),Scalar(),true);
    Mat blob = blobFromImage(color, 1/100.0,
                                  Size(300, 300),Scalar(127.5,127.5,127.5),true,true); //Convert Mat to batch of images

    neuralNetwork.setInput(blob);
    Mat detections = neuralNetwork.forward("detection_out");
    Mat matricesDet(detections.size[2],detections.size[3],CV_32F,detections.ptr<float>());

    color = color(crop);
    depth = depth(crop);

    float confidenceThreshold = 0.4f;
    for(int i = 0; i < matricesDet.rows; i++)
    {
        float confidence = matricesDet.at<float>(i, 2);

        if(confidence > confidenceThreshold)
        {
            size_t objectClass = (size_t)(matricesDet.at<float>(i, 1));

            int xLeftBottom = static_cast<int>(matricesDet.at<float>(i, 3) * color.cols);
            int yLeftBottom = static_cast<int>(matricesDet.at<float>(i, 4) * color.rows);
            int xRightTop = static_cast<int>(matricesDet.at<float>(i, 5) * color.cols);
            int yRightTop = static_cast<int>(matricesDet.at<float>(i, 6) * color.rows);

            Rect object((int)xLeftBottom, (int)yLeftBottom,
                        (int)(xRightTop - xLeftBottom),
                        (int)(yRightTop - yLeftBottom));

            object = object  & cv::Rect(0, 0, depth.cols, depth.rows);

            // Calculate mean depth inside the detection region
            // This is a very naive way to estimate objects depth
            // but it is intended to demonstrate how one might
            // use depht data in general
            Scalar m = mean(depth(object));
            object.x += startingPos.x;
            object.y += startingPos.y;

            std::string nom = "";
            nom = main ? "Hand":classNames[objectClass];

            DetectedObject obj(object,nom,m[0],confidence);
            objets.push_back(obj);

        }
    }
    return objets;


}
