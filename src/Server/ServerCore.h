/*
 * ServerCore.h
 *
 *  Created on: 2016年3月22日
 *      Author: xiang
 */

#ifndef SRC_SERVER_SERVERCORE_H_
#define SRC_SERVER_SERVERCORE_H_
#include <iostream>
using namespace std;
#include "SocketUtils/Socket.h"
#include "SocketUtils/ServerSocket.h"
#include "Server/DoubleConnector.h"
#include <arpa/inet.h>
#include <vector>
#include "Thread/Thread.h"
#include <fstream>
#include <time.h>
#include <unistd.h>

#define DEBUG 0
#define DEEPDEBUG 0
class ServerCore {
public:

	static ServerCore* getInstance();

	void run();

	virtual ~ServerCore();

	DoubleConnector _doubleConnector;

	ofstream *_ofstream;

	char * getCurrentTime();

	ServerSocket* getServerSocket();

	int _errorTimeCount;
private:
	ServerCore();

	ServerSocket* _serverSocket;

	vector<Thread> *_threadVec;

	sockaddr_in _serverAddress;
};

static ServerCore *_serverCoreInstance=NULL;

class TimerThread:public Thread
{
public:
	bool _isClose;
	TimerThread():Thread()
	{
		_isClose=false;
		_count=0;
	}
	TimerThread(function<void* (void* arg)> runnable):Thread(runnable)
	{
		_isClose=false;
		_count=0;
	}
	int _count;
};

#endif /* SRC_SERVER_SERVERCORE_H_ */
