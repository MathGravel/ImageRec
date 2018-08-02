/* 
 * File:   MainWindow.hpp
 * Author: imam
 *
 * Created on 25 September 2015, 7:52 PM
 */

#ifndef MAINWINDOW_HPP
#define    MAINWINDOW_HPP

// Gtkmm includes
#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <ctime>
#include <atomic>
#include <mutex>
#include <thread>
#include <chrono>
#include <gtkmm.h>
#include <gtkmm/offscreenwindow.h>
#include "FileManager.h"
#include "ImageSegmentationManual.h"
#include "VideoSources/RealSense.h"
#include "VideoSources/RealSenseVideo.h"
#include "Policy.h"
#include "VideoArea.hpp"

class MainWindow : public Gtk::Window {
public:
    MainWindow();

    ~MainWindow();

protected:

    VideoArea video_area;

    Gtk::Frame video_frame;
    Gtk::Frame activity_frame;
    Gtk::Frame plan_frame;
    Gtk::Frame log_frame;
    Gtk::Frame prev_act;
    Gtk::Frame act_act;
    Gtk::Frame fut_act;
    Gtk::Grid activities_grid;


    Gtk::Entry prev_act_text;
    Gtk::Entry act_act_text;
    Gtk::Entry next_act_text;
    Gtk::Grid next_grid;
    Gtk::Grid prev_grid;
    Gtk::Grid act_grid;
    Gtk::Label prev_lbl;
    Gtk::Label act_lbl;
    Gtk::Label next_lbl;
    Gtk::Entry prev_prob;
    Gtk::Entry act_prob;
    Gtk::Entry next_prob;
    Gtk::Label prev_prob_lbl;
    Gtk::Label act_prob_lbl;
    Gtk::Label next_prob_lbl;


    Gtk::Label planLabel;
    Gtk::Entry currentPlan;
    Gtk::Label planProbLabel;
    Gtk::Entry currentPlanProb;
    Gtk::Grid plan_grid;
    Gtk::Button startCapture;

    Gtk::TextView logger;
    Gtk::ScrolledWindow scrolled;
    Gtk::Button stopCapture;
    Gtk::Button showAnalysis;
    Gtk::Button testButton;
    Gtk::Button savePicButton;
    Gtk::Button localRecButton;
    Gtk::Button globalRecButton;
    Gtk::Entry activity;
    Gtk::Entry activity2;
    Gtk::Box box;
    Gtk::Grid grid;
    Gtk::FlowBox actions;
    Gtk::TextView log;
    Gtk::ButtonBox bx;
    Gtk::Label a;

    void on_parsing_error(const Glib::RefPtr<const Gtk::CssSection> &section, const Glib::Error &error);

    void start_thread();

    void start_affordance_check();

    void on_button_clicked(Glib::ustring data);

    void on_start_capture(Glib::ustring data);

    void on_stop_capture(Glib::ustring data);

    void on_show_analysis(Glib::ustring data);

    void on_save_pic(Glib::ustring data);

    void on_local_recognition(Glib::ustring data);

    void on_global_recognition(Glib::ustring data);

    bool on_stop_press(GdkEventButton *data);


private:
    std::mutex mtx;
    std::atomic<bool> isStopped;
    VideoSource *cameraFeed;
    std::string previous;
    Policy p;
    ActivityRegion *act;
    bool segImg = false;

    void pack_View();

    void destroy(GdkEventAny *event);

    void setActivityRegion(Gtk::Frame &region);

    void setPlanRegion(Gtk::Frame &region);

    void setLogRegion(Gtk::Frame &region);

    void setImageRegion(Gtk::Frame &region);


};

#endif    /* MAINWINDOW_HPP */

