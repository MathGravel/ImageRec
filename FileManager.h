//
// Created by uqamportable on 28/01/18.
//

#ifndef IMAGEREC_FILEMANAGER_H
#define IMAGEREC_FILEMANAGER_H

#include <gtkmm/dialog.h>
#include <gtkmm/window.h>

#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/grid.h>



class FileManager : public Gtk::Dialog {

public:
    FileManager(Gtk::Window* parent);

    std::string getCategorieItem() {
        return categorieEntry.get_text();
    }

protected:
    void on_save(Glib::ustring data);
    void on_cancel(Glib::ustring data);
    void destroy (GdkEventAny* event);


private:
    Gtk::Button yesButton;
    Gtk::Button noButton;
    Gtk::Entry categorieEntry;
    Gtk::Grid layout;



};


#endif //IMAGEREC_FILEMANAGER_H
