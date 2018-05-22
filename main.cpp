
// GTKmm and User interface includes
#include "MainWindow.hpp"

int main1(int argc, char** argv)
{
    Policy p;
    p.update(Affordance());
    p.getNextAction();
    return 0;
}



int main(int argc, char** argv)
{
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv);

    MainWindow mwindow;
    app->run (mwindow);


    return 0;
}

