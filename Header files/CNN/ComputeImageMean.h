
#pragma once
#include <string>
#include <stdint.h>
#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "boost/scoped_ptr.hpp"





using std::max;
using std::pair;
using boost::scoped_ptr;




class ComputeImageMean {
public:
	ComputeImageMean();
	std::string createMeanImage();


private:
	std::string lmdbPath;
	std::string meanPath;


};