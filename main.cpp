
// GTKmm and User interface includes
#include "MainWindow.hpp"
#include "FileManager.h"
#include <gtkmm/application.h>

int main(int argc, char** argv)
{
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv);

    MainWindow mwindow;
    //FileManager mg;
    app->run (mwindow);

    //mwindow.Test();
    //video_thread.join ();
    
    return 0;
}

