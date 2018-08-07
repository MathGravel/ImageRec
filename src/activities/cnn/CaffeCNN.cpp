#include "CaffeCNN.h"


#ifdef USE_OPENCV_CAFFE


using namespace caffe;  // NOLINT(build/namespaces)
using std::string;




CaffeCNN::CaffeCNN()
{


    string model_file   = "./InfosCNN/deploy.prototxt";
    string trained_file = "./InfosCNN/solver_iter_2000.caffemodel";
    //string trained_file = "./ImageRec/cnnModel/newcnn.caffemodel";
    string mean_file    = "./InfosCNN/agewell_mean.binaryproto";
    string label_file   = "./InfosCNN/det_synset_words.txt";
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

std::map<std::string, float> CaffeCNN::predictMatrix(const cv::Mat& picture)
{
    std::map<std::string, float> nul;
    return nul;
}

Prediction CaffeCNN::predict(const cv::Mat & picture)
{
    std::vector<Prediction> predictions = classifier.Classify(picture);
    /* Print the top N predictions. */
    /*for (size_t i = 0; i < predictions.size(); ++i) {
        Prediction p = predictions[i];
        oss << std::fixed << std::setprecision(4) << p.second << " - \""
            << p.first << "\"" << std::endl;
    }*/

    return predictions[0];
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

#endif
