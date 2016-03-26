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
#include <arpa/inet.h>
class ServerCore {
public:

	static ServerCore* getInstance();

	void run();

	virtual ~ServerCore();


private:
	ServerCore();

	ServerSocket* _serverSocket;

	int _clientFileDescriptor;

	sockaddr_in _serverAddress;

};

static ServerCore *_serverCoreInstance=NULL;

#endif /* SRC_SERVER_SERVERCORE_H_ */
