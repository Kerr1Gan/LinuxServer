/*
 * SocketUtil.cpp
 *
 *  Created on: 2016年3月21日
 *      Author: xiang
 */

#include "SocketUtil.h"

SocketUtil::SocketUtil() {
	// TODO Auto-generated constructor stub

}

SocketUtil::~SocketUtil() {
	// TODO Auto-generated destructor stub
}

void SocketUtil::run() {
	int socket_fd, connect_fd;
	sockaddr_in servaddr;
	char buff[4096];
	int n;
	//初始化Socket
	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}
	//初始化
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址。
	servaddr.sin_port = htons(DEFAULT_PORT); //设置的端口为DEFAULT_PORT

	//将本地地址绑定到所创建的套接字上
	if (bind(socket_fd, (struct sockaddr*) &servaddr, sizeof(servaddr)) == -1) {
		printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}
	//开始监听是否有客户端连接
	if (listen(socket_fd, 10) == -1) {
		printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}
	printf("======waiting for client's request======\n");

	if ((connect_fd = accept(socket_fd, (sockaddr*) NULL, NULL)) == -1) {
		printf("accept socket error: %s(errno: %d)", strerror(errno),
		errno);
	}

	while (true) {

		n = recv(connect_fd, buff, MAXLINE, 0);

		printf("%s\n", buff);
		send(connect_fd, "Hello,you are connected!\n", 26, 0);

		if (memcmp(buff, "exit", strlen("exit")) == 0) {
			break;
		}
	}

	shutdown(socket_fd, 2);
	shutdown(connect_fd, 2);
}

int SocketUtil::bytesToInt(char *byte)
{
		unsigned char* bys;
		bys=(unsigned char*)byte;
		int result;
		unsigned char bytes[5];
		memset(bytes,0,5);

		bytes[0]=*(byte);
		bytes[1]=*(byte+1);
		bytes[2]=*(byte+2);
		bytes[3]=*(byte+3);

		result=((bytes[0] )| ((bytes[1]<<8) &0xff00)
				| ((bytes[2] <<16) & 0xff0000)| ((bytes[3] <<24)&0xff000000));

		return result;
}


void SocketUtil::intToBytes(int integer,char result[5])
{
		unsigned char* res;
		res=(unsigned char*)result;
		memset(result,0,5);

		result[0]=(integer)&0xff;
		result[1]=(integer>>8)&0xff;
		result[2]=(integer>>16)&0xff;
		result[3]=(integer>>24)&0xff;

}

sockaddr_in SocketUtil::getLocalHost()
{
	sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	return address;
}

char* SocketUtil::getSocketIPandPort(int fileDescriptor,int &port)
{
	socklen_t len=sizeof(sockaddr_in	);
	sockaddr_in addr;

	char *ip=NULL;
	port=0;

	if(getpeername(fileDescriptor,(sockaddr*)&addr,&len)==0)
	{
		ip=inet_ntoa(addr.sin_addr);
		port=ntohs(len);
	}
	return ip;
}
bool SocketUtil::isIPValid(char *ip)
{
	   if (ip == NULL || *ip == '\0')
	      return 1;

	   union
	   {
	      struct sockaddr addr;
	      struct sockaddr_in6 addr6;
	      struct sockaddr_in addr4;
	   } addr;

	memset(&addr,0,sizeof(addr));

	if (inet_pton(AF_INET, NULL, &addr.addr4.sin_addr) <= 0) {
		printf("inet_pton error for \n");
		return false;
	}
	else if(inet_pton (AF_INET6, ip, &addr.addr6.sin6_addr)<=0){
		printf("inet_pton error for \n");
		return false;
	}

	return true;
}

