/*
 * Mutex.h
 *
 *  Created on: 2016年3月23日
 *      Author: xiang
 */

#ifndef __MUTEX_H__
#define  __MUTEX_H__

#include <iostream>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
using namespace std;

class Mutex {
public:
	Mutex();
	virtual ~Mutex();

	pthread_mutex_t getMutex();
	bool isLock();
	void lock();
	void unLock();

	bool isWaitting();
	void wait();
	void notify();
	void notifyAll();
private:
	pthread_mutex_t _mutex;
	pthread_cond_t _conditional;
	bool _isLock;
	bool _isWaitting;
protected:
	bool lock(bool isLock);
	bool wait(bool isWaitting);
};

#endif

