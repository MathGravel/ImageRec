//
// Created by uqamportable on 28/01/18.
//

#include "FileManager.h"


FileManager::FileManager(Gtk::Window* parent,std::string previous) {

    set_title ("Camera Feed");
    set_border_width (10);

    //this->set_size_request(1920,1080);
    layout = Gtk::Grid();
    yesButton = Gtk::Button("Save");
    noButton = Gtk::Button("Cancel");
    categorieEntry = Gtk::Entry();
    categorieEntry.set_text(previous);
    this->set_transient_for(*parent);
    //layout.attach(categorieEntry,0,0,2,1);
    //layout.attach(yesButton,0,1,1,1);
    //layout.attach_next_to(noButton,yesButton,Gtk::PositionType::POS_RIGHT,1,1);

    //add(layout);
    add_action_widget(categorieEntry,3);

    add_button("Save",1);
    add_button("Cancel",2);
    show_all();
    //show_all_children(true);

}