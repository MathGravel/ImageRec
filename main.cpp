
// GTKmm and User interface includes
#include "MainWindow.hpp"




int main(int argc, char** argv)
{
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv);

    MainWindow mwindow;
    //FileManager mg;
    app->run (mwindow);


    return 0;
}

