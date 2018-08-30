#include "YoloGPU.h"
#include <unistd.h>
extern "C" {
#include <darknet.h>
}

YoloGPU::YoloGPU( float _prob) {


    cfg = cfgbuff;
    fil = filbuff;
    wei = weibuff;
    dat = datbuff;
    lbl = lblbuff;

    strncpy(dat,"ressources/models/yolo.data",256);
    strncpy(lbl,"ressources/models/classes.name",600);
    strncpy(cfg,"ressources/models/yolov3.cfg",256);
    strncpy(wei,"ressources/models/yolov3.backup",256);

    names = get_labels(lbl);


    net = load_network(cfg,wei, 0);
    set_batch_network((network*)net, 1);
    srand(2222222);
    nms =.45;
    thresh = 0.5;

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
    detection *dets = get_network_boxes((network*)net, im.w, im.h, thresh, 0.5, 0, 1, &nboxes);
    if (nms) do_nms_sort(dets, nboxes, l.classes, nms);

    std::vector<DetectedObject> objects;


    for (i = 0; i < nboxes;i++) {
        bool correct = false;
        int pos = -1;
        for (int j = 0; j <10;j++) {
            if (dets[i].prob[j] > thresh) {
                correct = true;
                pos = j;
            }
        }
        if (correct) {
            cv::Rect obj(0,0,0,0);
            auto b = dets[i].bbox;
            int left  = (b.x-b.w/2.)*im.w;
            int right = (b.x+b.w/2.)*im.w;
            int top   = (b.y-b.h/2.)*im.h;
            int bot   = (b.y+b.h/2.)*im.h;

            if(left < 0) left = 0;
            if(right > im.w-1) right = im.w-1;
            if(top < 0) top = 0;
            if(bot > im.h-1) bot = im.h-1;
            obj.x = left;
            obj.y = top;
            obj.width = right - left;
            obj.height = bot - top;

            /*int temp = (dets[i].bbox.x - dets[i].bbox.w/2.) * im.w;
            obj.x = temp > 0? temp : 0;
            temp = (dets[i].bbox.y - dets[i].bbox.h/2.) * im.h;
            obj.y = temp > 0? temp : 0;
            temp = (dets[i].bbox.x + dets[i].bbox.w/2.) * im.w;
            obj.width = temp < color.rows  ? temp - obj.x : color.rows-1;
            temp = (dets[i].bbox.y + dets[i].bbox.h/2.) * im.h;
            obj.height = temp < color.cols  ? temp - obj.y : color.cols-1;*/




            cv::Scalar m = mean(depth(obj));
            std::string nom = names[pos];
            int offset = pos*123457 % 10;
            float red = get_color(2,offset,10);
             float green = get_color(1,offset,10);
             float blue = get_color(0,offset,10);
              float rgb[3];
              rgb[0] = red;
              rgb[1] = green;
              rgb[2] = blue;
            objects.emplace_back(DetectedObject(obj,nom,m[0],dets[i].prob[pos],red,green,blue));
        }
    }
    std::vector<bool> posi(objects.size());
    std::fill(posi.begin(),posi.end(),true);

    std::vector<DetectedObject> objs;
    for (int i = 0; i < objects.size();i++) {
        DetectedObject obj(objects[i]);
        std::cout << obj.getObjName() << std::endl;
        if (posi[i]) {
            for (int j = i + 1; j < objects.size();j++) {
                if (obj.getObjName() != objects[j].getObjName())
                    continue;
                int ar = (obj.getObjPos() & objects[j].getObjPos()).area();
                if ((ar * 1.3 > obj.getObjPos().area() || ar * 1.3 > objects[j].getObjPos().area()) &&
                        obj.getObjName() == objects[j].getObjName()) {
                    posi[i] = false;
                    posi[j] = false;
                    obj.fusePosition(objects[j]);
                }
            }
            objs.push_back(obj);
        }

    }

    free_detections(dets, nboxes);
    free_image(im);
    free_image(sized);
    //free_layer(l);
    return objs;
}

float colors[6][3] = { {255,0,255}, {0,0,255},{0,255,255},{0,255,0},{255,255,0},{255,0,0} };

float YoloGPU::get_color(int c, int x, int max)
{
    float ratio = ((float)x/max)*5;
    int i = floor(ratio);
    int j = ceil(ratio);
    ratio -= i;
    float r = (1-ratio) * colors[i][c] + ratio*colors[j][c];
    //printf("%f\n", r);
    return r;
}
