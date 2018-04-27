#include "Threading.h"
#include <vector>
#include <algorithm>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

using namespace std;

vector<boost::thread*> Threading::ThreadList;

boost::thread* Threading::StartThread(boost::function<void()> func){
	boost::thread* thread = new boost::thread(func);
	Threading::ThreadList.push_back(thread);
	return thread;
}

void Threading::CloseThread(boost::thread* thread){
	thread->interrupt();
}

void Threading::CloseAllThreads(){
	for(auto const& thread: Threading::ThreadList){
		thread->interrupt();
	}

	Threading::ThreadList.clear();
}