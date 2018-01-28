#include "MainWindow.hpp"

MainWindow::MainWindow()
{

    set_title ("Camera Feed");
    set_border_width (10);


    box = Gtk::Box(Gtk::ORIENTATION_VERTICAL);


    this->set_size_request(1920,1080);
    bx  = Gtk::ButtonBox(Gtk::ORIENTATION_HORIZONTAL);
    video_frame.set_label ("OpenCV Video");
    video_frame.set_label_align (Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
    video_frame.set_shadow_type (Gtk::SHADOW_OUT);
    video_frame.set_size_request(1620, 680);
    //this->set_size_request(900,800);
    video_area.set_tooltip_text("Video");
    video_frame.add(video_area);
    startCapture = Gtk::Button("Start");
    stopCapture = Gtk::Button("Stop");
    showAnalysis = Gtk::Button("Show Analysis");
    testButton = Gtk::Button("Test");
    testButton.set_size_request(200,200);
    //box.add(video_frame);
    box.add(video_frame);
    //bx.add(video_frame);
    bx.add(startCapture);
    bx.add(stopCapture);
    bx.add(showAnalysis);

    bx.show();
    box.add(bx);
    add(box);
    //bx = Gtk::HButtonBox(L);
   // bx.add(startCapture);
   // bx.add(stopCapture);
   // bx.pack_start(startCapture);
    //bx.set_size_request(400,400);
    //a = Gtk::Label("This is the first",true);
   // a.set_size_request(200,200);
    //Gtk::Label b("This is the second",true);
    //add(a);
    //add(a);
   // bx.add(a);
    //bx.add(b);
    //bx.add(showAnalysis);
    //bx.add(testButton);
    //box.add(bx);
    //box.add(startCapture);
    //box.add(stopCapture);
    //box.add(showAnalysis);
    //box.add(testButton);
    //box.add(bx);
    // Now when the button is clicked, we call the "on_button_clicked" function
    // with a pointer to "button 1" as it's argument
    startCapture.signal_clicked().connect(sigc::bind<Glib::ustring>(
            sigc::mem_fun(*this, &MainWindow::on_start_capture), "Start Capture"));
    stopCapture.signal_clicked().connect(sigc::bind<Glib::ustring>(
            sigc::mem_fun(*this, &MainWindow::on_stop_capture), "Stop Capture"));
    showAnalysis.signal_clicked().connect(sigc::bind<Glib::ustring>(
            sigc::mem_fun(*this, &MainWindow::on_show_analysis), "show Analysis"));

   // add(box);
   //add(bx);
     //this->signal_delete_event().connect(sigc::mem_fun(this,&MainWindow::destroy));
   // startCapture.show();
    //add(startCapture);
    //startCapture.show();
    //box.show();
    //video_frame.show();
    //video_frame.show_all_children();
    //add(video_frame);
    //show();
    show_all();
    show_all_children(true);
    //startCapture.show();
    //bx.show();

    //show_all_children(true);*/
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
    video_area.StartKinect();
}

// Our new improved signal handler.  The data passed to this method is
// printed to stdout.
void MainWindow::on_stop_capture(Glib::ustring data)
{
    std::cout << "Hello World - " << data << " was pressed" << std::endl;
    video_area.StopKinect();
}


// Our new improved signal handler.  The data passed to this method is
// printed to stdout.
void MainWindow::on_show_analysis(Glib::ustring data)
{
    std::cout << "Hello World - " << data << " was pressed" << std::endl;
    FileManager fm(this);
    int result =  fm.run();

}


void MainWindow::Test() {
    video_area.StartKinect();

}

MainWindow::~MainWindow()
{

}