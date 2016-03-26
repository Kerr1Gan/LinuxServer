/*
 * Thread.h
 *
 *  Created on: 2016年3月23日
 *      Author: xiang
 */

#ifndef SRC_THREAD_THREAD_H_
#define SRC_THREAD_THREAD_H_

#include <iostream>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "Thread/Mutex.h"
#include <functional>
using namespace std;


typedef void* (*CallBack) (void* arg);

class Thread {
public:
	Thread();
	virtual ~Thread();

	Mutex getMutex();

	pthread_t getThreadID();

	static pthread_t getCurrentThreadID();

	void lock();

	void unlock();

	void wait();

	void notify();

	void notifyAll();

	void setRunnable(CallBack runnable);

	void start();

	void cancle();

	bool isLock();

	bool isWait();

	void* join();

	function<void* (void* arg)>  _bindRunnabe;

//	Thread(CallBack runnable);

	Thread(function<void* (void* arg)> runnable);

	void setArg(void *p);

	void* getArg();

	void setTag(string tag);

	string getTag();
protected:
	CallBack getRunnable();
	static void* run(void*);
private:
		Mutex _mutex;
		pthread_t _thread;

		CallBack _runnable;

		void* _arg;

		string _tag;
};

#endif /* SRC_THREAD_THREAD_H_ */
