#include "../../Header files/CNN/ImgSegCNN.h"
#include <caffe/caffe.hpp>


ImgSegCNN::ImgSegCNN()
{

    string model_file   = "/home/uqamportable/CLionProjects/ImageRec/InfosCNN/deploy.prototxt";
    string trained_file = "/home/uqamportable/CLionProjects/ImageRec/InfosCNN/sol.caffemodel";
    string mean_file    = "/home/uqamportable/CLionProjects/ImageRec/InfosCNN/agewell_mean.binaryproto";
    string label_file   = "/home/uqamportable/CLionProjects/ImageRec/InfosCNN/det_synset_words.txt";


     neuralNet = readNetFromCaffe(model_file,trained_file);
    train();
}

ImgSegCNN::~ImgSegCNN()
{
}

void ImgSegCNN::train()
{
    caffe::BlobProto blob_proto;
    caffe::ReadProtoFromBinaryFileOrDie("/home/uqamportable/CLionProjects/ImageRec/InfosCNN/agewell_mean.binaryproto", &blob_proto);

    /* Convert from BlobProto to Blob<float> */
    caffe::Blob<float> mean_blob;
    mean_blob.FromProto(blob_proto);

    /* The format of the mean file is planar 32-bit float BGR or grayscale. */
    std::vector<cv::Mat> channels;
    float* data = mean_blob.mutable_cpu_data();
    for (int i = 0; i < 3; ++i) {
        /* Extract an individual channel. */
        cv::Mat channel(mean_blob.height(), mean_blob.width(), CV_32FC1, data);
        channels.push_back(channel);
        data += mean_blob.height() * mean_blob.width();
    }

    /* Merge the separate channels into a single image. */
    cv::Mat mean;
   // std::vector<cv::Mat> channe;
    //channe.push_back(channels[2]);
    //channe.push_back(channels[1]);
    //channe.push_back(channels[0]);


    cv::merge(channels, mean);

    /* Compute the global mean pixel value and create a mean image
    * filled with this value. */
     channel_mean = cv::mean(mean);
    check = mean.type();
//    mean_ = cv::Mat(input_geometry_, mean.type(), channel_mean);

}

std::string ImgSegCNN::getPictureInfo(const cv::Mat & image)
{
	return std::string();
}

void ImgSegCNN::updateModel(const cv::Mat & picture, bool correctlyIdentified)
{
}

std::string ImgSegCNN::predict(const cv::Mat & picture)
{

    cv::Mat pic;

    cv::resize(picture,pic,cv::Size(256,256));
    cv::imwrite("testing.png",pic);
    //GoogLeNet accepts only 224x224 BGR-images
    Mat inputBlob = blobFromImage(pic, 1.0f, Size(256,256),
                                  cv::Scalar(104,117,123), false,false);   //Convert Mat to batch of images
    //! [Prepare blob]
    neuralNet.setInput(inputBlob, "data");        //set the network input
    Mat prob = neuralNet.forward("prob");         //compute output

    //! [Gather output]
    int classId;
    double classProb;
    getMaxClass(prob, &classId, &classProb);//find the best class

    //! [Print results]
    std::vector<String> classNames = readClassNames("/home/uqamportable/CLionProjects/ImageRec/InfosCNN/det_synset_words.txt");
    std::cout << "Best class: #" << classId << " '" << classNames.at(classId) << "'" << std::endl;
    std::cout << "Probability: " << classProb * 100 << "%" << std::endl;
    //! [Print results]


    string val = classNames.at(classId) + " " + ( classProb * 100) + " ";
	return val;
}

void ImgSegCNN::savePicture(const cv::Mat & picture, std::string name)
{
}


std::vector<double> ImgSegCNN::getClassesProb(const Mat &probBloc) {

    std::vector<double> classesProb;
    std::cout << probBloc.at<float>(1,0);
    int j = max(probBloc.cols,probBloc.rows);
    for (int i = 0; i < j; i++)
        classesProb.push_back(probBloc.at<float>(0,i) * 100);

    return classesProb;


}



/* Find best class for the blob (i. e. class with maximal probability) */
 void ImgSegCNN::getMaxClass(const Mat &probBlob, int *classId, double *classProb)
{
    Mat probMat = probBlob.reshape(1, 1); //reshape the blob to 1x1000 matrix
    Point classNumber;


    minMaxLoc(probMat, NULL, classProb, NULL, &classNumber);

    *classId = classNumber.x;
}

 std::vector<String> ImgSegCNN::readClassNames(const char *filename )
{
    std::vector<String> classNames;

    std::ifstream fp(filename);
    if (!fp.is_open())
    {
        std::cerr << "File with classes labels not found: " << filename << std::endl;
        exit(-1);
    }

    std::string name;
    while (!fp.eof())
    {
        std::getline(fp, name);
        if (name.length())
            classNames.push_back( name.substr(name.find(' ')+1) );
    }

    fp.close();
    return classNames;
}
