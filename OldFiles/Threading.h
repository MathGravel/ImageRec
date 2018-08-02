#ifndef IMAGEREC_THREADING_H
#define IMAGEREC_THREADING_H

#include <vector>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

using namespace std;

class Threading{
	public: 
		static vector<boost::thread*> ThreadList;
		static boost::thread* StartThread(boost::function<void()> func);
		static void CloseThread(boost::thread* thread);
		static void CloseAllThreads();
};

#endif //IMAGEREC_THREADING_H