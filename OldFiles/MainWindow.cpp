#include "MainWindow.hpp"

MainWindow::MainWindow()
{

    set_title ("Camera Feed");
    set_border_width (10);
    previous = "val";

    box = Gtk::Box(Gtk::ORIENTATION_VERTICAL);
    act = ActivityRegion::instance();
    set_name("Main-Window");

    this->set_size_request(1820,980);
    this->grid = Gtk::Grid();
    this->grid.set_column_homogeneous(true);
    grid.set_row_homogeneous(true);

    bx  = Gtk::ButtonBox(Gtk::ORIENTATION_HORIZONTAL);



    this->setImageRegion(video_frame);
    this->setLogRegion(log_frame);
    this->setPlanRegion(plan_frame);
    this->setActivityRegion(activity_frame);
    grid.attach(video_frame,1,1,4,3);
    grid.attach_next_to(log_frame,video_frame,Gtk::POS_RIGHT,1,3);
    grid.attach_next_to(activity_frame,video_frame,Gtk::POS_BOTTOM,4,1);
    grid.attach_next_to(plan_frame,activity_frame,Gtk::POS_RIGHT,1,1);



    add(grid);


    auto css = Gtk::CssProvider::create();

    if(not css->load_from_path("/home/uqamportable/CLionProjects/ImageRec/Css/Frame.css")) {
        cerr << "Failed to load css\n";
        std::exit(1);
    }
    auto screen = Gdk::Screen::get_default();
    auto ctx = this->get_style_context();
    ctx->add_provider_for_screen(screen, css, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    startCapture = Gtk::Button("Start");
    startCapture.set_name("test");





    show_all();
    show_all_children(true);

    cameraFeed = NULL;

    isStopped.store(false);
    start_thread();

}


void MainWindow::setActivityRegion(Gtk::Frame& region) {

    region.set_shadow_type(Gtk::ShadowType::SHADOW_ETCHED_OUT);
    prev_act.set_name("Previous-Action");
    act_act.set_name("Actual-Action");
    fut_act.set_name("Future-Action");
    //prev_act.set_label("Prev-Action");
    //act_act.set_label("Act-Action");
    //fut_act.set_label("Next-Action");
    //prev_act.add(prev_act_text);
    //act_act.add(act_act_text);
    //fut_act.add(next_act_text);
    prev_lbl.set_text("Previous Action");
    act_lbl.set_text("Current Action");
    next_lbl.set_text("Anticipated Action");
    prev_prob_lbl.set_text("Prob");
    act_prob_lbl.set_text("Prob");
    next_prob_lbl.set_text("Prob");


    prev_grid.attach(prev_lbl,1,1,1,1);
    prev_grid.attach_next_to(prev_act_text,prev_lbl,Gtk::POS_BOTTOM,1,1);
    prev_grid.attach_next_to(prev_prob_lbl,prev_act_text,Gtk::POS_BOTTOM,1,1);
    prev_grid.attach_next_to(prev_prob,prev_prob_lbl,Gtk::POS_BOTTOM,1,1);


    act_grid.attach(act_lbl,1,1,1,1);
    act_grid.attach_next_to(act_act_text,act_lbl,Gtk::POS_BOTTOM,1,1);
    act_grid.attach_next_to(act_prob_lbl,act_act_text,Gtk::POS_BOTTOM,1,1);
    act_grid.attach_next_to(act_prob,act_prob_lbl,Gtk::POS_BOTTOM,1,1);


    next_grid.attach(next_lbl,1,1,1,1);
    next_grid.attach_next_to(next_act_text,next_lbl,Gtk::POS_BOTTOM,1,1);
    next_grid.attach_next_to(next_prob_lbl,next_act_text,Gtk::POS_BOTTOM,1,1);
    next_grid.attach_next_to(next_prob,next_prob_lbl,Gtk::POS_BOTTOM,1,1);


    region.set_label ("Activities");

    region.set_border_width(4);

    activities_grid.attach(prev_act,0,0,2,1);
    activities_grid.attach_next_to(act_act,prev_act,Gtk::POS_RIGHT,2,1);
    activities_grid.attach_next_to(fut_act,act_act,Gtk::POS_RIGHT,2,1);


    prev_act.add(prev_grid);
    act_act.add(act_grid);
    fut_act.add(next_grid);
    region.add(activities_grid);

    activities_grid.show_all();
    region.show_all();

    prev_act_text.signal_button_press_event().connect(
            sigc::mem_fun(*this, &MainWindow::on_stop_press) );
    act_act_text.signal_button_press_event().connect(
            sigc::mem_fun(*this, &MainWindow::on_stop_press));
    next_act_text.signal_button_press_event().connect(
            sigc::mem_fun(*this, &MainWindow::on_stop_press));

    prev_act_text.set_sensitive(true);

    prev_act_text.set_property("editable", false);
    prev_act_text.set_property("cursor-visible", false);
    act_act_text.set_property("editable", false);
    act_act_text.set_property("cursor-visible", false);
    next_act_text.set_property("editable", false);
    next_act_text.set_property("cursor-visible", false);

}

void MainWindow::setPlanRegion(Gtk::Frame& region) {
    startCapture.set_label("Start");
    planLabel = Gtk::Label("Current Plan");
    plan_grid.attach(planLabel,1,1,1,1);
    plan_grid.attach_next_to(currentPlan,planLabel,Gtk::POS_BOTTOM,1,1);
    plan_grid.attach_next_to(planProbLabel,currentPlan,Gtk::POS_BOTTOM,1,1);
    plan_grid.attach_next_to(currentPlanProb,planProbLabel,Gtk::POS_BOTTOM,1,1);
    plan_grid.attach_next_to(startCapture,currentPlanProb,Gtk::POS_BOTTOM,1,1);
    region.add(plan_grid);
    //startCapture = Gtk::Button("Start");
    startCapture.signal_clicked().connect(sigc::bind<Glib::ustring>(
            sigc::mem_fun(*this, &MainWindow::on_start_capture), "Start Capture"));

}
void MainWindow::setLogRegion(Gtk::Frame& region) {
    region.set_label ("Log");
    scrolled.add(logger);
    region.add(scrolled);

}

void MainWindow::setImageRegion(Gtk::Frame& region) {

    region.set_label ("OpenCV Video");
    //region.set_label_align (Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
    region.set_shadow_type (Gtk::SHADOW_OUT);

    region.set_size_request(1280, 720);
    region.set_tooltip_text("Video");
    region.add(video_area);
}




MainWindow::~MainWindow() {
    isStopped.store(true);
    if (cameraFeed != NULL)
        delete cameraFeed;

    cameraFeed = NULL;
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

            std::string old_text = act_act_text.get_text();
            std::string old_prob = act_prob.get_text();
            std::string test = aff->getAffordance().getName();

            p.update(aff->getAffordance());


            if (old_text.front() != test.front()) {
                prev_act_text.set_text(old_text);
                prev_prob.set_text(old_prob);

                Glib::RefPtr<Gtk::TextBuffer> buffer = logger.get_buffer();
                std::stringstream ss;
                std::time_t t = std::time(0);   // get time now
                std::tm* now = std::localtime(&t);
                ss << (now->tm_year + 1900) << '-'
                   << (now->tm_mon + 1) << '-'
                   <<  now->tm_mday
                   << ' ' << now->tm_hour << ' ' << now->tm_min << ' ' << now->tm_sec  <<":\n";
                ss << "Current Action : " << aff->getAffordance().getName() << " " << floor(aff->getAffordance().getObjectProbability() * 100) << std::endl;
                ss << "Current Plan " << p.getCurrentPlan() << " " << p.getCurrentPlanProb() << std::endl;
                ss << "Next possible actions " << p.getNextAction() << std::endl;

                Gtk::TextBuffer::iterator iter;
                iter = buffer->get_iter_at_offset(0);
                iter = buffer->insert(iter, ss.str());

            }

            act_act_text.set_text(aff->getAffordance().getName());
            act_prob.set_text(std::to_string(aff->getAffordance().getObjectProbability()));


            std::string acts = p.getNextAction();

            next_act_text.set_text(acts.substr(0, acts.find(' ')));
            next_prob.set_text(acts.substr(acts.find(' ')));

            currentPlan.set_text(p.getCurrentPlan());
            currentPlanProb.set_text(p.getCurrentPlanProb());


            mtx.lock();
            mtx.unlock();

        }
        std::this_thread::sleep_for(std::chrono::microseconds(15));
    }
}

void  MainWindow::destroy (GdkEventAny* event)
{

    gtk_main_quit ();
}

bool MainWindow::on_stop_press(GdkEventButton* data)  {
    return false;
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

void MainWindow::on_parsing_error(const Glib::RefPtr<const Gtk::CssSection>& section, const Glib::Error& error)
{
    std::cerr << "on_parsing_error(): " << error.what() << std::endl;
    if (section)
    {
        const auto file = section->get_file();
        if (file)
        {
            std::cerr << "  URI = " << file->get_uri() << std::endl;
        }

        std::cerr << "  start_line = " << section->get_start_line()+1
                  << ", end_line = " << section->get_end_line()+1 << std::endl;
        std::cerr << "  start_position = " << section->get_start_position()
                  << ", end_position = " << section->get_end_position() << std::endl;
    }
}

