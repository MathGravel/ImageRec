#include "stdafx.h"
#include "GoogleVisionCNN.h"
#include <Python.h>


GoogleVisionCNN::GoogleVisionCNN()
{
	Py_Initialize();
	py::object io = py::import("io");
	py::object os = py::import("os");
	py::object vision = py::import("google.cloud.vision");
	py::object types = py::import("google.cloud.vision.types");
	py::object client = py::exec("vision.ImageAnnotatorClient()");


}
GoogleVisionCNN::~GoogleVisionCNN() {
	Py_Finalize();
}

void GoogleVisionCNN::train()
{
	//Not needed with the CNN used in network
}

std::string GoogleVisionCNN::getPictureInfo(const cv::Mat & image)
{
	py::object client = py::exec("client = vision.ImageAnnotatorClient()");
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

	/*for (int i = 0; i < size; i++) {
		std::string val = "act = labels[" + std::to_string(i) + "]";
		py::object item = py::exec(val.c_str());
		py::object itemAct = py::eval("act.description");
		std::string desc = py::extract<std::string>(itemAct);
		itemAct = py::eval("act.score");
		float score = py::extract<float>(itemAct);
	}*/
	return lastName;
}

void GoogleVisionCNN::updateModel(const cv::Mat & picture, bool correctlyIdentified)
{
}
