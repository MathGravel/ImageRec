
#pragma once
#include <string>
#include <stdint.h>
#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "boost/scoped_ptr.hpp"
#include "gflags/gflags.h"
#include "glog/logging.h"

#include "caffe/proto/caffe.pb.h"
#include "caffe/util/db.hpp"
#include "caffe/util/io.hpp"

using namespace caffe;  // NOLINT(build/namespaces)



using std::max;
using std::pair;
using boost::scoped_ptr;

DEFINE_string(backend, "lmdb",
	"The backend {leveldb, lmdb} containing the images");


class ComputeImageMean {
public:
	ComputeImageMean();
	std::string createMeanImage();


private:
	std::string lmdbPath;
	std::string meanPath;


};