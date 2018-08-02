//
// Created by uqamportable on 12/03/18.
//

#include "CNN/MaskRCNN.h"

MaskRCNN::MaskRCNN(std::string inference_path, int imgHeight,int imgWidth,bool estMain, float _prob) {

    width = 640;
    height = 480;
    imgWidth = 640;
    imgHeight = 480;

    resizeRatio = width / (float) height;
    network = inference_path + "/graph.pb";
    confidenceThreshold = _prob;
    networkDef = inference_path + "/label.pbtxt";
    main = estMain;
    neuralNetwork = cv::dnn::readNetFromTensorflow(network,networkDef);
    classNames = nullptr;

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

     std::ifstream inputFile(inference_path + "/classes.name");        // Input file stream object

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

MaskRCNN::~MaskRCNN() = default;

std::vector<DetectedObject> MaskRCNN::findObjects(cv::Mat color,cv::Mat depth) {

    std::vector<DetectedObject> objets;



    Mat blob;

    //color = color(crop);
    //depth = depth(crop);


    if (main)
         blob = blobFromImage(color, 1.0/100.0f,
                                  Size(256, 256),Scalar(127.5,127.5,127.5),true,true); //Convert Mat to batch of images
    else
         blob = blobFromImage(color, 1.0f/200.0f,
                                 Size(300, 300),Scalar(127.5,127.5,127.5),true,false); //Convert Mat to batch of images


    neuralNetwork.setInput(blob);
    Mat detections = neuralNetwork.forward("detection_out");
    Mat matricesDet(detections.size[2],detections.size[3],CV_32F,detections.ptr<float>());

    for(int i = 0; i < matricesDet.rows; i++)
    {
        float confidence = matricesDet.at<float>(i, 2);

        if(confidence > confidenceThreshold)
        {

            size_t objectClass = (size_t)(matricesDet.at<float>(i, 1));


            auto xLeftBottom = static_cast<int>(matricesDet.at<float>(i, 3) * color.cols);
            auto yLeftBottom = static_cast<int>(matricesDet.at<float>(i, 4) * color.rows);
            auto xRightTop = static_cast<int>(matricesDet.at<float>(i, 5) * color.cols);
            auto yRightTop = static_cast<int>(matricesDet.at<float>(i, 6) * color.rows);

            Rect object((int)xLeftBottom, (int)yLeftBottom,
                        (int)(xRightTop - xLeftBottom),
                        (int)(yRightTop - yLeftBottom));

            if (main) {
                if (object.area() > 50000)
                    continue;
            }

            object = object  & cv::Rect(0, 0, color.cols, color.rows);


            Scalar m = mean(depth(object));

            //object.x += startingPos.x;
           // object.y += startingPos.y;

            std::string nom = classNames[objectClass-1];

            DetectedObject obj(object,nom,m[0],0.8);
            objets.push_back(obj);
        }
    }
    return objets;


}
