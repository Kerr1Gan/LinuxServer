/*
 * ServerSocket.cpp
 *
 *  Created on: 2016年3月24日
 *      Author: xiang
 */

#include <ServerSocket.h>

ServerSocket::ServerSocket() {
	// TODO Auto-generated constructor stub

	_hostAddress= getAddress();
	_hostFileDescripotr=getFileDescriptor();
	//初始化Socket
	if ((_hostFileDescripotr = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
	}

}

ServerSocket::~ServerSocket() {
	// TODO Auto-generated destructor stub
}

void ServerSocket::bind()
{
	//将本地地址绑定到所创建的套接字上
	if (::bind(_hostFileDescripotr, (struct sockaddr*) &_hostAddress, sizeof(_hostAddress)) == -1) {
		printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno);
	}
}

void ServerSocket::bindDefault()
{
	//初始化
	memset(&_hostAddress, 0, sizeof(_hostAddress));
	_hostAddress.sin_family = AF_INET;
	_hostAddress.sin_addr.s_addr = htonl(INADDR_ANY); //IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址。
	_hostAddress.sin_port = htons(DEFAULT_PORT); //设置的端口为DEFAULT_PORT

	setIP(inet_ntoa(_hostAddress.sin_addr));
	setPort(DEFAULT_PORT);

	bind();
}

void ServerSocket::bind(char* ip,int port)
{
	//初始化
	memset(&_hostAddress, 0, sizeof(_hostAddress));
	_hostAddress.sin_family = AF_INET;
	_hostAddress.sin_addr.s_addr = inet_addr(ip);
	_hostAddress.sin_port = htons(port);
	setIP(ip);
	setPort(port);

	bind();
}

void ServerSocket::listen(int queueLength)
{
	//开始监听是否有客户端连接
	if (::listen(_hostFileDescripotr, 10) == -1) {
		printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
	}
}

int ServerSocket::accept()
{
	int clientFileDescriptor=0;
	if ((clientFileDescriptor = ::accept(_hostFileDescripotr, (sockaddr*) NULL, NULL)) == -1) {
		printf("accept socket error: %s(errno: %d)", strerror(errno),errno);
	}
	return clientFileDescriptor;
}

