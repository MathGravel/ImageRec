
// GTKmm and User interface includes
#include "MainWindow.hpp"
#include <Python.h>
#include <iostream>
#include <fstream>
#include <string>




int main(int argc, char** argv)
{
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv);

    MainWindow mwindow;
    //FileManager mg;
    app->run (mwindow);

    //mwindow.Test();
    //video_thread.join ();

   // PyObject *pName, *pModule, *pDict, *pFunc;
    //PyObject *pArgs, *pValue;

    /*Py_Initialize();
    pName = PyUnicode_FromString("io");
    pModule = PyImport_Import(pName);
    pName = PyUnicode_FromString("os");
    pModule = PyImport_Import(pName);
    pName = PyUnicode_FromString("getopt");
    pModule = PyImport_Import(pName);
    pName = PyUnicode_FromString("sys");
    pModule = PyImport_Import(pName);
    pName = PyUnicode_FromString("google.protobuf");
    pModule = PyImport_Import(pName);
    pName = PyUnicode_FromString("google.cloud.vision");
    pModule = PyImport_Import(pName);
    PyRun_SimpleString("from google.cloud import vision");
    PyRun_SimpleString("from google.cloud.vision import types");

    PyRun_SimpleString("client = vision.ImageAnnotatorClient()");


    Py_Finalize();
    */


    return 0;
}

