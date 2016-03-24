/*
 * ServerSocket.h
 *
 *  Created on: 2016年3月24日
 *      Author: xiang
 */

#ifndef SRC_SOCKETUTILS_SERVERSOCKET_H_
#define SRC_SOCKETUTILS_SERVERSOCKET_H_
#include "SocketUtils/Socket.h"

#define DEFAULT_PORT 8000

class ServerSocket :public Socket
{
public:
	ServerSocket();
	virtual ~ServerSocket();

	void bind(char* ip,int port);

	void bindDefault();

	void listen(int queueLength);

	int accept();

private:
	sockaddr_in _hostAddress;
	int _hostFileDescripotr;
protected:
	void bind();
};

#endif /* SRC_SOCKETUTILS_SERVERSOCKET_H_ */
