#include "../../Header files/CNN/CaffeCNN.h"




using namespace caffe;  // NOLINT(build/namespaces)
using std::string;




CaffeCNN::CaffeCNN()
{


    //::google::InitGoogleLogging("ImageRec");

    string model_file   = "/home/uqamportable/CLionProjects/ImageRec/InfosCNN/deploy.prototxt";
    string trained_file = "/home/uqamportable/CLionProjects/ImageRec/InfosCNN/caffenet_train_iter_500.caffemodel";
    //string trained_file = "/home/uqamportable/CLionProjects/ImageRec/cnnModel/newcnn.caffemodel";
    string mean_file    = "/home/uqamportable/CLionProjects/ImageRec/InfosCNN/imagenet_mean.binaryproto";
    string label_file   = "/home/uqamportable/CLionProjects/ImageRec/InfosCNN/det_synset_words.txt";
    classifier = Classifier(model_file, trained_file, mean_file, label_file);




}


CaffeCNN::~CaffeCNN()
{
}

void CaffeCNN::train() {
		
}

std::string CaffeCNN::getPictureInfo(const cv::Mat & image)
{

    std::vector<Prediction> predictions = classifier.Classify(image);
    std::ostringstream oss;
    /* Print the top N predictions. */
    for (size_t i = 0; i < predictions.size(); ++i) {
        Prediction p = predictions[i];
        oss << std::fixed << std::setprecision(4) << p.second << " - \""
                  << p.first << "\"" << std::endl;
    }

	return oss.str();
}

void CaffeCNN::updateModel(const cv::Mat & picture, bool correctlyIdentified)
{
}

std::string CaffeCNN::predict(const cv::Mat & picture)
{
    std::vector<Prediction> predictions = classifier.Classify(picture);
    std::ostringstream oss;
    /* Print the top N predictions. */
    /*for (size_t i = 0; i < predictions.size(); ++i) {
        Prediction p = predictions[i];
        oss << std::fixed << std::setprecision(4) << p.second << " - \""
            << p.first << "\"" << std::endl;
    }*/
    oss << std::fixed << std::setprecision(4) << predictions[0].second << "-"
        << predictions[0].first  << std::endl;
    return oss.str();
}

void CaffeCNN::savePicture(const cv::Mat & picture, std::string name)
{
}

void CaffeCNN::trainCNN()
{
}

void CaffeCNN::prepareData()
{
}

void CaffeCNN::setImageMean(const std::string mean_loc)
{
}

std::vector<float> CaffeCNN::predire(const cv::Mat & img)
{
	return std::vector<float>();
}

void CaffeCNN::wrapInputLayer(std::vector<cv::Mat>* input_channels)
{
}

void CaffeCNN::preparation(const cv::Mat & img, std::vector<cv::Mat>* input_channels)
{
}

