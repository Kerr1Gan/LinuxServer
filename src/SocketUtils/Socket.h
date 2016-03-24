/*
 * Socket.h
 *
 *  Created on: 2016年3月24日
 *      Author: xiang
 */

#ifndef SRC_SOCKETUTILS_SOCKET_H_
#define SRC_SOCKETUTILS_SOCKET_H_
#include <iostream>

#include <string>
#include <cstdlib>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include<netinet/in.h>
#include<errno.h>
#include<sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
using namespace std;

class Socket {
public:
	Socket();
	virtual ~Socket();

	int getPort();

	string getIP();

	int connect(char* ip,int port);

	int send(char* content,int length);

	int receive(char* content,int length);

	int &getFileDescriptor();

	void close();

	void setConnectTimeOut(int millionSeconds);

	int getConnectTimeOut();
protected:

	sockaddr_in &getAddress();

	void setIP(char *ip);

	void setPort(int port);
private:

	int _fileDescriptor;

	sockaddr_in _address;

	int _port;

	string _ip;
};

#endif /* SRC_SOCKETUTILS_SOCKET_H_ */
