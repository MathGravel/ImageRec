#include "../Header files/stdafx.h"
#include "../Header files/FileManager.h"


FileManager::FileManager()
{
	compression_param.push_back(FILE_FORMAT);
	compression_param.push_back(9); 
}


FileManager::~FileManager()
{
}

void FileManager::saveImage(std::string categorie, cv::Mat & mat, TypeSaver type)
{
	switch (type) {
	case 1:
		saveImage(categorie, mat); 
		break;
	case 2:
		break;
	default:
		break; 
	}
}

void FileManager::saveImage(std::string categorie, cv::Mat& mat)
{
	std::stringstream path;
	path << createFolderPath(categorie); 
	std::wstring folder = s2ws(path.str());

	if (CreateDirectory(folder.c_str(), NULL) ||
		ERROR_ALREADY_EXISTS == GetLastError()) {

		path << createFilename(categorie);
		std::string superpath = path.str();
		cv::Mat dest = cv::Mat::zeros(cv::Size(FILE_SQUARE, FILE_SQUARE), mat.type());
		normalizeSize(mat, dest);
		if (true) {// && false) {
			try {
				imwrite(superpath, dest, compression_param);
			}
			catch (std::runtime_error& ex) {
				std::string msg(ex.what());
				DBOUT("Le fichier n'as pas ete sauvegarde" << msg);
			}
			DBOUT("Fichier sauvegarde");
		}
	}
	//DBOUT("FilenameCreated : " << createFilename(categorie)); 
	DBOUT(path.str() << std::endl); 
}

std::string FileManager::createFilename(std::string categorie)
{
	char c;
	std::stringstream filename(""); 
	filename << categorie << "_"; 
	filename << createActualTimeString();
	filename << ".png";
	return filename.str();
}

std::string FileManager::createFolderPath(std::string categorie) 
{
	std::stringstream pathName(""); 
	pathName << "C:\\filetosave\\";
	pathName << categorie << "\\";
	return pathName.str();
}


std::string FileManager::createActualTimeString()
{
	std::stringstream timestring(""); 
	std::chrono::system_clock::time_point tp = 
		std::chrono::system_clock::now(); 
	std::stringstream timePointString(""); 
	std::time_t timeIs; 
	timeIs = std::chrono::system_clock::to_time_t(tp); 
	timePointString << ctime(&timeIs); 
	char c; 
	while (timePointString.get(c)) {
		if (c == ':' || c == ' ')
			timestring << '_';
		else if (c == '\n')
			NULL;
		else
			timestring << c; 
	}
	return timestring.str(); 
}

cv::Mat FileManager::normalizeSize(cv::Mat& mat, cv::Mat& dest)
{
	int width = mat.cols; 
	int height = mat.rows; 
	// cv::Mat dest1 = cv::Mat::zeros(cv::Size(FILE_SQUARE, FILE_SQUARE), mat.type()); 
	int max_dim = (width >= height) ? width : height; 
	float scale = ((float)FILE_SQUARE) / max_dim;
	cv::Rect roi; 
	if (width >= height)
	{
		roi.width = FILE_SQUARE;
		roi.x = 0;
		roi.height = height * scale;
		roi.y =( FILE_SQUARE - roi.height) / 2;
	}
	else
	{
		roi.y = 0; 
		roi.height = FILE_SQUARE; 
		roi.width = width * scale; 
		roi.x = (FILE_SQUARE - roi.width) / 2; 
	}

	cv::resize(mat,dest(roi), roi.size());
	return dest;
}

// https://stackoverflow.com/questions/27220/how-to-convert-stdstring-to-lpcwstr-in-c-unicode
std::wstring FileManager::s2ws(const std::string& s)
{
	int len; 
	int slength = (int)s.length() + 1; 
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len); 
	std::wstring r(buf); 
	delete[] buf; 
	return r;
}