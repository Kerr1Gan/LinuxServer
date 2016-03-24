/*
 * Socket.cpp
 *
 *  Created on: 2016年3月24日
 *      Author: xiang
 */

#include "SocketUtils/Socket.h"

Socket::Socket() {
	// TODO Auto-generated constructor stub
	_fileDescriptor=0;
	_address={0};
	_port=0;
}

Socket::Socket(int sock):Socket()
{
	_fileDescriptor=sock;
}

Socket::~Socket() {
	// TODO Auto-generated destructor stub
	if(_fileDescriptor!=0)
		close();
}

int &Socket::getFileDescriptor()
{
	return _fileDescriptor;
}

sockaddr_in &Socket::getAddress()
{
	return _address;
}

int  Socket::getPort()
{
	return _port;
}

string  Socket::getIP()
{
	return _ip;
}

int  Socket::connect(char* ip,int port)
{
	if ((_fileDescriptor = ::socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	memset(&_address, 0, sizeof(_address));
	_address.sin_family = AF_INET;
	_address.sin_port = htons(port);
	_address.sin_addr.s_addr=inet_addr(ip);

	_ip=ip;
	_port=port;

	int res=::connect(_fileDescriptor, (struct sockaddr*) &_address, sizeof(_address));
	if ( res < 0) {
		printf("Connect error: %s(errno: %d)\n", strerror(errno), errno);
	}

	return res;
}

int Socket::send(char* content,int length)
{
	int res=0;
	if ( (res=::send(_fileDescriptor, content, length, 0) )< 0)
	{
		printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
	}
	return res;
}

int Socket::receive(char* content,int length)
{
	int res;
	if (( res = ::recv(_fileDescriptor, content, length, 0)) == -1) {
		perror("recv error");
	}
	return res;
}

void Socket::close()
{
	::shutdown(_fileDescriptor,2);
	_fileDescriptor=0;
}

void Socket::setIP(char *ip)
{
	_ip=ip;
}

void Socket::setPort(int port)
{
	_port=port;
}

void Socket::setConnectTimeOut(int microSeconds)
{
	timeval val;
	val.tv_sec=0;
	val.tv_usec=microSeconds;

	setsockopt(_fileDescriptor,SOL_SOCKET,SO_RCVTIMEO,&val,sizeof(val));
}

int Socket::getConnectTimeOut()
{
	timeval val;
	socklen_t len=sizeof(val);
	getsockopt(_fileDescriptor,SOL_SOCKET,SO_RCVTIMEO,&val,&len);
	return len;
}
