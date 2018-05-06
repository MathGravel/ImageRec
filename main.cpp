
// GTKmm and User interface includes
#include "MainWindow.hpp"
#include "Affordance.h"

#include "Policy.h"


int main1(int argc, char** argv)
{
    Policy p;

    //std::cout << Affordance() << std::endl;
    p.getNextAction();
    return 0;
}



int main(int argc, char** argv)
{
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv);

    MainWindow mwindow;
    //FileManager mg;
    app->run (mwindow);


    return 0;
}

