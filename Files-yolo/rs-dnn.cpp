// This example is derived from the ssd_mobilenet_object_detection opencv demo
// and adapted to be used with Intel RealSense Cameras
// Please see https://github.com/opencv/opencv/blob/master/LICENSE

#include <opencv2/dnn.hpp>
#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>   // Include OpenCV API
#include "../cv-helpers.hpp"   // Include OpenCV API
#include <vector>
#include <iostream>
#include <fstream>
extern "C" {
#include "darknet.h"
}
#include <highgui.h>


const size_t inWidth      = 300;
const size_t inHeight     = 300;
const float WHRatio       = inWidth / (float)inHeight;
const float inScaleFactor = 0.007843f;
const float meanVal       = 127.5;
const char* classNames[]  = {"hand",
"background"};

int main(int argc, char** argv) try
{
    using namespace rs2;
    using namespace cv;
    using namespace cv::dnn;
    std::string nn;
    std::string networkDef;
    nn = "yolov3-hand.cfg";
    networkDef = "yolov3-hand_final.weights";
    Net nett = cv::dnn::readNetFromDarknet(nn,networkDef);

    // Start streaming from Intel RealSense Camera
    pipeline pipe;
    rs2::config ccfg;
    ccfg.enable_stream(RS2_STREAM_COLOR,1280,720,RS2_FORMAT_BGR8,30);
    ccfg.enable_stream(RS2_STREAM_DEPTH,1280,720,RS2_FORMAT_Z16);
    auto config = pipe.start(ccfg);
    auto profile = config.get_stream(RS2_STREAM_COLOR)
    .as<video_stream_profile>();
    rs2::align align_to(RS2_STREAM_COLOR);

    Size cropSize;
    if (profile.width() / (float)profile.height() > WHRatio)
    {
        cropSize = Size(static_cast<int>(profile.height() * WHRatio),
            profile.height());
    }
    else
    {
        cropSize = Size(profile.width(),
            static_cast<int>(profile.width() / WHRatio));
    }

    Rect crop(Point((profile.width() - cropSize.width) / 2,
        (profile.height() - cropSize.height) / 2),
    cropSize);
    
    const auto window_name = "Display Image";
    namedWindow(window_name, WINDOW_AUTOSIZE);

    char cfgbuff[256];
    char filbuff[256];
    char weibuff[256];
    char datbuff[256];
    char lblbuff[256];

    char* cfg = cfgbuff;
    char *fil = filbuff;
    char * wei = weibuff;
    char *dat = datbuff;
    char *lbl = lblbuff;
    strncpy(dat,"/home/troisiememathieu/Music/YOLO/darknet/cfg/voc-hand.data",256);
    std::cout << "1234" << std::endl;
    strncpy(lbl,"/home/troisiememathieu/Documents/AgeWell_DataSet_Tensorflow/nouvelles/mouvementmain/transform/classes.name",256);
    strncpy(cfg,"/home/troisiememathieu/Music/YOLO/darknet/cfg/yolov3-hand.cfg",256);
    strncpy(wei,"yolov3-hand_final.weights",256);

    list *options = read_data_cfg(dat);
    
    char **names = get_labels(lbl);

    image **alphabet = load_alphabet();
    network *net = load_network(cfg,wei, 0);
    set_batch_network(net, 1);
    srand(2222222);
    double time;
    char buff[256];
    char *input = buff;
    float nms=.45;
    layer l = net->layers[net->n-1];
    


    while (true)
    {
        // Wait for the next set of frames
        auto data = pipe.wait_for_frames();
        // Make sure the frames are spatially aligned
        data = align_to.process(data);

        auto color_frame = data.get_color_frame();
        auto depth_frame = data.get_depth_frame();

        // If we only received new depth frame, 
        // but the color did not update, continue
        static int last_frame_number = 0;
        if (color_frame.get_frame_number() == last_frame_number) continue;
        last_frame_number = color_frame.get_frame_number();

        // Convert RealSense frame to OpenCV matrix:
        auto color_mat = frame_to_mat(color_frame);
        auto depth_mat = depth_frame_to_meters(pipe, depth_frame);

        /*Mat inputBlob = blobFromImage(color_mat, inScaleFactor,
                                      Size(inWidth, inHeight), meanVal, false); //Convert Mat to batch of images
        net.setInput(inputBlob, "data"); //set the network input
        Mat detection = net.forward("detection_out"); //compute output
        


        Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());
        */
        //color_mat = color_mat(crop);
        //depth_mat = depth_mat(crop);
        //imwrite("test.png",color_mat);
	//imwrite("test.png",color_mat);
	cv::Mat color;// = color_mat;
	//cv::cvtColor(color_mat, color, COLOR_RGB2BGR);

        image im = make_image (color_mat.size().width,color_mat.size().height,color_mat.channels());//= load_image_color(fil,0,0);
        {
        int h = color_mat.size().height;
    	int w = color_mat.size().width;
    	int c = 3;
    	int step = color_mat.step;
          int i, j, k;
          for(i = 0; i < h; ++i){
            for(k= 0; k < c; ++k){
                for(j = 0; j < w; ++j){
                    im.data[k*w*h + i*w + j] = color_mat.at<unsigned char>(i * step + j*c + k)/255.;
                }
            }
        }
        
    }
	std::cerr << "Verif ";
    	//cvReleaseImage(&&img);
    	//rgbgr_image(out);
    int i;
    for(i = 0; i < im.w*im.h; ++i){
        float swap = im.data[i];
        im.data[i] = im.data[i+im.w*im.h*2];
        im.data[i+im.w*im.h*2] = swap;
    }


    image sized = letterbox_image(im, net->w, net->h);

        //image sized = resize_image(im, net->w, net->h);
        //image sized2 = resize_max(im, net->w);
        //image sized = crop_image(sized2, -((net->w - sized2.w)/2), -((net->h - sized2.h)/2), net->w, net->h);
        //resize_network(net, sized.w, sized.h);



    float *X = sized.data;
    time=what_time_is_it_now();
    network_predict(net, X);
    printf("%s: Predicted in %f seconds.\n", input, what_time_is_it_now()-time);
    int nboxes = 0;
    detection *dets = get_network_boxes(net, im.w, im.h, 0.5, 0.5, 0, 1, &nboxes);
	

        //printf("%d\n", nboxes);
        //if (nms) do_nms_obj(boxes, probs, l.w*l.h*l.n, l.classes, nms);

    if (nms) do_nms_sort(dets, nboxes, l.classes, nms);
    //std::cout << dets[0].prob[0] << std::endl;

    draw_detections(im, dets, nboxes, 0.5, names, alphabet, l.classes+1);
    free_detections(dets, nboxes);
    //save_image(im, "predictions");
        unsigned char* buffer = new unsigned char[color_mat.total() * color_mat.elemSize()];
    {

        int h = color_mat.size().height;
    	int w = color_mat.size().width;
    	int c = 3;
    	int step = color_mat.step;
          int i, j, k;
          for(i = 0; i < h; ++i){
            for(k= 0; k < c; ++k){
                for(j = 0; j < w; ++j){
                    buffer[i * step + j*c + k] = im.data[k*w*h + i*w + j] * 255.;
                }
            }
        }
        
    }


   cv::Mat newPic(Size(color_mat.size().width, color_mat.size().height), color_mat.type(),buffer,Mat::AUTO_STEP);
		cv::cvtColor(newPic, newPic, COLOR_BGR2RGB);   
delete buffer;
    

    //cv::Mat newPic = imread("predictions.png");
    imshow(window_name, newPic);
    free_image(im);
    free_image(sized);

        //imshow(window_name, color_mat);
    if (waitKey(1) >= 0) break;
}

return EXIT_SUCCESS;
}
catch (const rs2::error & e)
{
    std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
    return EXIT_FAILURE;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}



