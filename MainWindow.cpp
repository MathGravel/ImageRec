#include "MainWindow.hpp"

MainWindow::MainWindow()
{

    set_title ("Camera Feed");
    set_border_width (10);
    previous = "val";

    box = Gtk::Box(Gtk::ORIENTATION_VERTICAL);
    act = ActivityRegion::instance();

    this->set_size_request(1920,1080);
    bx  = Gtk::ButtonBox(Gtk::ORIENTATION_HORIZONTAL);
    video_frame.set_label ("OpenCV Video");
    video_frame.set_label_align (Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
    video_frame.set_shadow_type (Gtk::SHADOW_OUT);

    video_frame.set_size_request(1620, 780);
    //this->set_size_request(900,800);
    video_area.set_tooltip_text("Video");
    video_frame.add(video_area);
    startCapture = Gtk::Button("Start");
    stopCapture = Gtk::Button("Stop");
    showAnalysis = Gtk::Button("Show Analysis");
    savePicButton = Gtk::Button("Save ROI");
    localRecButton = Gtk::Button("Local Recognition");
    globalRecButton = Gtk::Button("Global Recognition");
    testButton.set_size_request(200,200);
    box.add(video_frame);
    //box.add(video_frame);
    //bx.add(video_frame);
    bx.set_spacing(10);
    box.set_spacing(10);

    bx.add(startCapture);
    bx.add(stopCapture);
    bx.add(showAnalysis);
    bx.add(savePicButton);
    bx.add(localRecButton);
    bx.add(globalRecButton);

    bx.show();

    box.add(bx);
    activity.set_text("Current Plan : \tProb:");
    box.add(activity);

    box.add(activity2);
    add(box);
    //bx = Gtk::HButtonBox(L);

    startCapture.signal_clicked().connect(sigc::bind<Glib::ustring>(
            sigc::mem_fun(*this, &MainWindow::on_start_capture), "Start Capture"));
    stopCapture.signal_clicked().connect(sigc::bind<Glib::ustring>(
            sigc::mem_fun(*this, &MainWindow::on_stop_capture), "Stop Capture"));
    showAnalysis.signal_clicked().connect(sigc::bind<Glib::ustring>(
            sigc::mem_fun(*this, &MainWindow::on_show_analysis), "show Analysis"));
    savePicButton.signal_clicked().connect(sigc::bind<Glib::ustring>(
            sigc::mem_fun(*this, &MainWindow::on_save_pic), "save picture"));
    localRecButton.signal_clicked().connect(sigc::bind<Glib::ustring>(
            sigc::mem_fun(*this, &MainWindow::on_local_recognition), "Local recognition"));
    globalRecButton.signal_clicked().connect(sigc::bind<Glib::ustring>(
            sigc::mem_fun(*this, &MainWindow::on_global_recognition), "Global Recognition"));

    show_all();
    show_all_children(true);
    //startCapture.show();
    //bx.show();

    //show_all_children(true);*/
    cameraFeed = NULL;

    isStopped.store(false);
    start_thread();
}

MainWindow::~MainWindow() {
    isStopped.store(true);
    if (cameraFeed != NULL)
        delete cameraFeed;
    if (kinTreat != NULL)
        delete kinTreat;
    cameraFeed = NULL;
    kinTreat = NULL;
}

void MainWindow::start_thread(){
    std::thread affordanceThread(&MainWindow::start_affordance_check, this);
    affordanceThread.detach();
}

void MainWindow::start_affordance_check(){
    while(!isStopped.load()){
        if(!act->currentAffordances.empty()){
            AffordanceTime* aff = act->currentAffordances.top();
            act->currentAffordances.pop();

            p.update(aff->getAffordance());
            std::string updated = aff->getAffordance().to_str() + " -> ";
            updated = updated  + p.getCurrentPlan();
            std::string acts = p.getNextAction();
            mtx.lock();
            activity.set_text(updated);
            activity2.set_text(acts);
            mtx.unlock();

        }
        std::this_thread::sleep_for(std::chrono::microseconds(15));
    }
}

void  MainWindow::destroy (GdkEventAny* event)
{
    gtk_main_quit ();
}

void MainWindow::pack_View(){

}


// Our new improved signal handler.  The data passed to this method is
// printed to stdout.
void MainWindow::on_start_capture(Glib::ustring data)
{
    std::cout << "Hello World - " << data << " was pressed" << std::endl;
    if (cameraFeed == NULL) {
        //cameraFeed = new Kinect();
        //cameraFeed = new RealSense();
        cameraFeed = new RealSenseVideo();
        video_area.StartCamera(cameraFeed);
        video_area.showi = false;
        video_area.setLocalSegmentation();
    } else
        video_area.RestartCamera();
}

// Our new improved signal handler.  The data passed to this method is
// printed to stdout.
void MainWindow::on_stop_capture(Glib::ustring data)
{
    std::cout << "Hello World - " << data << " was pressed" << std::endl;
    video_area.StopCamera();
}

void MainWindow::on_show_analysis(Glib::ustring data) {

    //video_area.setSegImage();
    video_area.SegmentClassifyROI();
}

// Our new improved signal handler.  The data passed to this method is
// printed to stdout.
void MainWindow::on_save_pic(Glib::ustring data)
{
    std::cout << "Hello World - " << data << " was pressed" << std::endl;
    FileManager fm(this,previous);
    int result =  fm.run();
    string location = "/home/uqamportable/Pictures";
    if (result == 1 ){
        previous = fm.getCategorieItem();
        video_area.SaveROI(location,fm.getCategorieItem());
    }

}

void MainWindow::on_local_recognition(Glib::ustring data) {

    video_area.showi = false;
    video_area.setLocalSegmentation();

}

void MainWindow::on_global_recognition(Glib::ustring data) {
    //video_area.SavePictures();
    Affordance aff = video_area.GetCurrentAffordance();
    p.update(aff);

    mtx.lock();
    activity.set_text(p.getNextAction());
    mtx.unlock();
}

