/*
 * Mutex.cpp
 *
 *  Created on: 2016年3月23日
 *      Author: xiang
 */

#include "Mutex.h"

Mutex::Mutex() {
	// TODO Auto-generated constructor stub
	pthread_mutex_init(&_mutex,NULL);
	pthread_cond_init(&_conditional,NULL);
	_isLock=false;
	_isWaitting=false;
}

Mutex::~Mutex() {
	// TODO Auto-generated destructor stub
	pthread_mutex_destroy(&_mutex);
	pthread_cond_destroy(&_conditional);
}


pthread_mutex_t Mutex:: getMutex()
{
		return _mutex;
}

bool  Mutex::isLock()
{
	return _isLock;
}

void  Mutex::lock()
{
	lock(true);
	pthread_mutex_lock(&_mutex);
}
void  Mutex::unLock()
{
	pthread_mutex_unlock(&_mutex);
	lock(false);
}
bool Mutex::lock(bool isLock)
{
	bool preState=isLock;
	_isLock=isLock;
	return preState;
}

bool Mutex::isWaitting()
{
	return _isWaitting;
}

bool Mutex::wait(bool isWaitting)
{
	bool preState=_isWaitting;
	_isWaitting=isWaitting;
	return  preState;
}

void Mutex::wait()
{
	this->wait(true);
	pthread_cond_wait(&_conditional,&_mutex);
}

void Mutex::notify()
{
	pthread_cond_signal(&_conditional);
	this->wait(false);
}

void Mutex::notifyAll()
{
	pthread_cond_broadcast(&_conditional);
	this->wait(false);
}
