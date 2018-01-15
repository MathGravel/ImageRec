#pragma once
#include <string>

class ConvertImageSet {

public:
	ConvertImageSet();


private:
	int width;
	int height;
	bool shuffle;
	std::string lmdbPath;

};