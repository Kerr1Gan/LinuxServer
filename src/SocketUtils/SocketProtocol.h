/*
 * SocketProtocol.h
 *
 *  Created on: 2016年3月21日
 *      Author: xiang
 */

#ifndef SRC_SOCKETUTILS_SOCKETPROTOCOL_H_
#define SRC_SOCKETUTILS_SOCKETPROTOCOL_H_
#include <iostream>
#include <stdlib.h>
#include <string>
#include <signal.h>
#include "SocketUtil.h"
#include "SocketUtils/Socket.h"
using namespace std;
#define DEFAULT_MAX_RECEIVE_BYTES  1024*10
#define DEFAULT_MAX_SEND_BYTES 1024*10
class SocketProtocol {
public:
	SocketProtocol();
	virtual ~SocketProtocol();


	static char* transferLocalStreamToNetWorkStream(char* localStream,int &length);

	static char* transferNetWorkStreamToLocalStream(char* networkStream,int length);

	static char* getNetWorkStreamToLocalStream(Socket* socket,int &len);

	static bool sendNetWorkStream(Socket* socket,char* content,int &len);

	static int getNetWorkStreamLength(char *netWorkStream);

	static int getNetWorkStreamLengthWidthHead(char *netWorkStream);

	static void setDefaultMaxReceiveBytes(int count);

	static void setDefaultMaxSendBytes(int count);
};

static int _defaultMaxReceiveBytes=DEFAULT_MAX_RECEIVE_BYTES;
static int _defaultMaxSendBytes=DEFAULT_MAX_SEND_BYTES;

#endif /* SRC_SOCKETUTILS_SOCKETPROTOCOL_H_ */
