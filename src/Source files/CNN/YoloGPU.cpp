//
// Created by uqamportable on 12/03/18.
//

#include "YoloGPU.h"

extern "C" {
#include <darknet.h>
}

YoloGPU::YoloGPU( float _prob) {


    cfg = cfgbuff;
    fil = filbuff;
    wei = weibuff;
    dat = datbuff;
    lbl = lblbuff;
    strncpy(dat,"../Files-yolo/voc-hand.data",256);
    strncpy(lbl,"../Files-yolo/classes.name",256);
    strncpy(cfg,"../Files-yolo/yolov3-hand.cfg",256);
    strncpy(wei,"../Files-yolo/yolov3-hand_final.weights",256);

    //options = read_data_cfg(dat);

    names = get_labels(lbl);

    net = load_network(cfg,wei, 0);
    set_batch_network((network*)net, 1);
    srand(2222222);
    nms =.45;
    thresh = _prob;

}

YoloGPU::~YoloGPU() {
    free_network((network*)net);

    //free_list(options);
}


std::vector<DetectedObject> YoloGPU::findObjects(cv::Mat color,cv::Mat depth) {

    image im = make_image (color.size().width,color.size().height,color.channels());//= load_image_color(fil,0,0);

    layer l = ((network*)net)->layers[((network*)net)->n-1];

    int h = color.size().height;
        int w = color.size().width;
        int c = 3;
        int step = color.step;
        int i, j, k;
        for(i = 0; i < h; ++i){
            for(k= 0; k < c; ++k){
                for(j = 0; j < w; ++j){
                    im.data[k*w*h + i*w + j] = color.at<unsigned char>(i * step + j*c + k)/255.;
                }
            }
        }

    for(i = 0; i < im.w*im.h; ++i){
        float swap = im.data[i];
        im.data[i] = im.data[i+im.w*im.h*2];
        im.data[i+im.w*im.h*2] = swap;
    }

    image sized = letterbox_image(im, ((network*)net)->w, ((network*)net)->h);

    //image sized = resize_image(im, net->w, net->h);
    //image sized2 = resize_max(im, net->w);
    //image sized = crop_image(sized2, -((net->w - sized2.w)/2), -((net->h - sized2.h)/2), net->w, net->h);
    //resize_network(net, sized.w, sized.h);



    float *X = sized.data;
    time=what_time_is_it_now();
    network_predict((network*)net, X);
    printf(" Predicted in %f seconds.\n",  what_time_is_it_now()-time);
    int nboxes = 0;
    detection *dets = get_network_boxes((network*)net, im.w, im.h, 0.5, 0.5, 0, 1, &nboxes);
    if (nms) do_nms_sort(dets, nboxes, l.classes, nms);

    std::vector<DetectedObject> objects;




    for (i = 0; i < nboxes;i++) {
        if (dets[i].prob[0] > thresh) {
            cv::Rect obj(0,0,0,0);
            int temp = (dets[i].bbox.x - dets[i].bbox.w/2.) * im.w;
            obj.x = temp > 0? temp : 0;
            temp = (dets[i].bbox.y - dets[i].bbox.h/2.) * im.h;
            obj.y = temp > 0? temp : 0;
            temp = (dets[i].bbox.x + dets[i].bbox.w/2.) * im.w;
            obj.width = temp < color.rows  ? temp - obj.x : color.rows-1;
            temp = (dets[i].bbox.y + dets[i].bbox.h/2.) * im.h;
            obj.width = temp < color.cols  ? temp - obj.y : color.cols-1;
            cv::Scalar m = mean(depth(obj));
            objects.emplace_back(DetectedObject(obj,"main",m[0]));
        }
    }


    free_detections(dets, nboxes);
    free_image(im);
    free_image(sized);
    //free_layer(l);
    return objects;
}
