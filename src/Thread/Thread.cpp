/*
 * Thread.cpp
 *
 *  Created on: 2016年3月23日
 *      Author: xiang
 */

#include "Thread.h"
#include "Macro.h"
Thread::Thread() {
	// TODO Auto-generated constructor stub
	_runnable=NULL;
	_thread=0;
	_arg=NULL;
	_tag="";
}

Thread::~Thread() {
	// TODO Auto-generated destructor stub
//	if(_arg!=NULL)
//		SAFE_RELEASE(_arg);
}

//Thread::Thread(CallBack runnable):Thread()
//{
//		_runnable=runnable;
//}

Thread::Thread(function<void*(void* arg)> runnable):Thread()
{
	_bindRunnabe=runnable;
}

Mutex Thread::getMutex()
{
	return _mutex;
}

pthread_t Thread::getThreadID()
{
	return _thread;
}

pthread_t Thread::getCurrentThreadID()
{
	return pthread_self();
}

void Thread::lock()
{
	_mutex.lock();
}

void Thread::unlock()
{
	_mutex.unLock();
}

void Thread::wait()
{
	_mutex.wait();
}

void Thread::notify()
{
	_mutex.notify();
}

void Thread::notifyAll()
{
	_mutex.notifyAll();
}

void Thread::setRunnable(CallBack runnable)
{
	_runnable=runnable;
}

void Thread::start()
{
	if (_bindRunnabe)
	{
		int res = pthread_create(&_thread, NULL, run, this);
		if (res != 0)
		{
			cout << "Create thread error.ID " << _thread << endl;
		}
//		run(this);
	}
	else if(_runnable!=NULL)
	{
		int res = pthread_create(&_thread, NULL, _runnable, this);
		if (res != 0)
		{
			cout << "Create thread error.ID " <<_thread << endl;
		}
	}
}

void* Thread::run(void* pt)
{
	auto owner=(Thread*)pt;
	if(owner->_bindRunnabe)
	{
		owner->_bindRunnabe(owner);
	}

	return owner;
}

void Thread::cancle()
{
	pthread_cancel(_thread);
}

CallBack Thread::getRunnable()
{
	return _runnable;
}

bool Thread::isLock()
{
	return _mutex.isLock();
}

bool Thread::isWait()
{
	return _mutex.isWaitting();
}

void Thread::setArg(void *p)
{
	_arg=p;
}

void* Thread::getArg()
{
	return _arg;
}

void* Thread::join()
{
	void* res=NULL;
	int isError=-1;
	isError=pthread_join(_thread,&res);
	if (isError != 0)
	{
		cout << "Join thread error.ID " <<_thread << endl;
	}
	return res;
}

void Thread::setTag(string tag)
{
	_tag=tag;
}

string Thread::getTag()
{
	return _tag;
}
