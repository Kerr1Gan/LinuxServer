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
#include <SocketUtils/SocketHeads.h>
#include <arpa/inet.h>
class ServerCore {
public:

	static ServerCore* getInstance();

	void run();

	virtual ~ServerCore();

	char* getClientIPandPort(int fileDescriptor,int &port);

private:
	ServerCore();

	int _serverFileDescriptor;

	int _clientFileDescriptor;

	sockaddr_in _serverAddress;

};

static ServerCore *_serverCoreInstance=NULL;

#endif /* SRC_SERVER_SERVERCORE_H_ */