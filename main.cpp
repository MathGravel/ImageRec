
// GTKmm and User interface includes
#include "MainWindow.hpp"
#include "Header files/CNN/ImgSegCNN.h"
#include <caffe/caffe.hpp>
#include <opencv2/opencv.hpp>


/*int main(int argc, char** argv)
{
    cv::Mat img;
    img = cv::imread("check.png");
    ImgSegCNN cnn;
    cnn.predict(img);


    return 0;
}*/



int main(int argc, char** argv)
{
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv);

    MainWindow mwindow;
    //FileManager mg;
    app->run (mwindow);


    return 0;
}

