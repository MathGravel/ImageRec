#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <chrono>
#include <opencv2/opencv.hpp>
#include "Dbout.h"

#define FILE_FORMAT CV_IMWRITE_PNG_COMPRESSION
#define FILE_DESTINATION "C:\\filetosave\\"
#define FILE_SQUARE 256
#define FORCING_SQUARE true


enum TypeSaver { SQUARE = 1, AS_SELECTED = 2, SQUARE_BORDER = 3 };
class FileManager
{
public:
	FileManager();
	~FileManager();
	
	std::vector<int> compression_param; 

	void saveImage(std::string categorie, cv::Mat& mat, TypeSaver type); 
	void saveImage(std::string path, cv::Mat& mat); 
	std::string createFilename(std::string categorie);
	std::string createFolderPath(std::string categorie);
	std::string createActualTimeString(); 
	cv::Mat normalizeSize(cv::Mat& mat, cv::Mat& dest);
	std::wstring s2ws(const std::string & s);
};
 