#include "../../Header files/stdafx.h"
#include "../../Header files/CNN/GoogleVisionCNN.h"
#include <Python.h>
#include <iostream>
#include <fstream>
#include <map>


#include <shlobj.h>

#pragma comment(lib, "shell32.lib")

GoogleVisionCNN::GoogleVisionCNN()
{
	//Py_Initialize();
	//py::object io = py::import("io");
	//py::object os = py::import("os");
	//py::object sys = py::import("sys");
	
	//PyImport_ImportModule(test.c_str());
	//PyRun_SimpleString("from google.cloud import vision");
	//py::object vision = py::import("google.cloud.vision");
	//py::object types = py::import("google.cloud.vision.types");
	//py::object client = py::exec("vision.ImageAnnotatorClient()");



}
GoogleVisionCNN::~GoogleVisionCNN() {
}

void GoogleVisionCNN::train()
{
	//Not needed with the CNN used in network
}

Prediction GoogleVisionCNN::getPictureInfo(const cv::Mat & image)
{
	/*py::object client = py::exec("client = vision.ImageAnnotatorClient()");
	std::vector<uchar> buf;
	cv::imencode(".png", image, buf);
	std::string buffer(buf.begin(),buf.end());
	std::string ret = "image = types.Image(content='" + buffer + "')";
	py::object img = py::exec(ret.c_str());
	py::object res = py::exec("response = client.label_detection(image = image)");
	py::object labels = py::exec("labels = response.label_annotations");
	py::object sizePy = py::eval("len(labels)");
	int size = py::extract<int>(sizePy);
	py::object item = py::exec("act = labels[0]");
	py::object itemAct = py::eval("act.description");

	lastName = py::extract<std::string>(itemAct);
	itemAct = py::eval("act.score");
	lastScore = py::extract<float>(itemAct);
	*/
	/*for (int i = 0; i < size; i++) {
		std::string val = "act = labels[" + std::to_string(i) + "]";
		py::object item = py::exec(val.c_str());
		py::object itemAct = py::eval("act.description");
		std::string desc = py::extract<std::string>(itemAct);
		itemAct = py::eval("act.score");
		float score = py::extract<float>(itemAct);
	}*/
	//TCHAR  my_documents[MAX_PATH];
	//HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);
	//std::wstring complete(my_documents);
	//std::string completePath(complete.begin(),complete.end());
	//completePath = completePath + "\\ImagesSaved";
	//std::string img = completePath + "\\test.png";
	//std::string py = completePath + "\\googleCloud.py";
	cv::imwrite("test.png", image);
	//std::string comm = "python " + img + " " + py;
	std::string comm = "python googleCloud.py \"test.png\"";

	system(comm.c_str());
	std::ifstream myfile("googleResult.txt");
	std::string line;
	if (myfile.is_open())
	{
		std::getline(myfile, line);
		lastName = line;
		//while (getline(myfile, line))
		//{
		//	cout << line << '\n';
		//}
		myfile.close();
	}


	return std::make_pair("",0);
}

void GoogleVisionCNN::updateModel(const cv::Mat & picture, bool correctlyIdentified)
{
}

std::string GoogleVisionCNN::predict(const cv::Mat & picture)
{
	return std::string();
}

void GoogleVisionCNN::savePicture(const cv::Mat & picture, std::string name)
{
}

void GoogleVisionCNN::trainCNN()
{
}
