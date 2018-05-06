/* 
 * File:   MainWindow.hpp
 * Author: imam
 *
 * Created on 25 September 2015, 7:52 PM
 */

#ifndef MAINWINDOW_HPP
#define	MAINWINDOW_HPP

// Gtkmm includes
#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <gtkmm/window.h>
#include <gtkmm/frame.h>
#include <gtkmm/button.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/grid.h>
#include <gtkmm/label.h>
#include <gtkmm/offscreenwindow.h>
#include "FileManager.h"
#include "ImageSegmentationManual.h"
#include "KinectTreatment.h"
#include "RealSense.h"
#include "Policy.h"

#include "Header files/CNN/CaffeCNN.h"




#include "VideoArea.hpp"

class MainWindow : public Gtk::Window
{
    public:
        MainWindow ();
    ~MainWindow();
        void Test();
    protected:
        Gtk::Frame video_frame;
        VideoArea video_area;
        Gtk::Button startCapture;
        Gtk::Button stopCapture;
        Gtk::Button showAnalysis;
        Gtk::Button testButton;
        Gtk::Button savePicButton;
        Gtk::Button localRecButton;
        Gtk::Button globalRecButton;
        Gtk::Entry activity;
        Gtk::Box box;
        Gtk::ButtonBox bx ;
        Gtk::Label a;
        void on_button_clicked(Glib::ustring data);
        void on_start_capture(Glib::ustring data);
        void on_stop_capture(Glib::ustring data);
        void on_show_analysis(Glib::ustring data);
        void on_save_pic(Glib::ustring data);
        void on_local_recognition(Glib::ustring data);
        void on_global_recognition(Glib::ustring data);




private:
        VideoSource * cameraFeed;
        VideoSource * camNoTreatment;
        std::string previous;
        KinectTreatment* kinTreat = NULL;
        Policy p;
        bool segImg = false;
        void pack_View();
        void destroy (GdkEventAny* event);

};

#endif	/* MAINWINDOW_HPP */

