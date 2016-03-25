/*
 * SocketUtil.h
 *
 *  Created on: 2016年3月21日
 *      Author: xiang
 */

#ifndef SRC_SOCKETUTILS_SOCKETUTIL_H_
#define SRC_SOCKETUTILS_SOCKETUTIL_H_
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <iostream>
#include <arpa/inet.h>
using namespace std;

#define DEFAULT_PORT 8000
#define MAXLINE 4096

class SocketUtil {
public:
	SocketUtil();
	virtual ~SocketUtil();

	static void run();

	static int bytesToInt(char *bytes);

	static void intToBytes( int integer,char result[5]);

	static sockaddr_in getLocalHost();

	static char* getSocketIPandPort(int fileDescriptor,int &port);

	static bool isIPValid(char *ip);
};

#endif /* SRC_SOCKETUTILS_SOCKETUTIL_H_ */
