/*
 * ServerCore.cpp
 *
 *  Created on: 2016年3月22日
 *      Author: xiang
 */

#include <Server/ServerCore.h>

ServerCore::ServerCore() {
	// TODO Auto-generated constructor stub
	_serverFileDescriptor=-1;
	_clientFileDescriptor=-1;
}

ServerCore::~ServerCore() {
	// TODO Auto-generated destructor stub
}

ServerCore* ServerCore::getInstance()
{
		if(_serverCoreInstance==NULL)
			_serverCoreInstance=new ServerCore();

		return _serverCoreInstance;
}

void ServerCore::run()
{
	char buff[4096];
	int n;
	//初始化Socket
	if ((_serverFileDescriptor = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}
	//初始化
	memset(&_serverAddress, 0, sizeof(_serverAddress));
	_serverAddress.sin_family = AF_INET;
	_serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); //IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址。
	_serverAddress.sin_port = htons(DEFAULT_PORT); //设置的端口为DEFAULT_PORT

	//将本地地址绑定到所创建的套接字上
	if (bind(_serverFileDescriptor, (struct sockaddr*) &_serverAddress, sizeof(_serverAddress)) == -1) {
		printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}
	//开始监听是否有客户端连接
	if (listen(_serverFileDescriptor, 10) == -1) {
		printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}
	printf("======waiting for client's request======\n");

	sockaddr clientAddress;

	if ((_clientFileDescriptor = accept(_serverFileDescriptor, (sockaddr*) NULL, NULL)) == -1) {
		printf("accept socket error: %s(errno: %d)", strerror(errno),
		errno);
	}

	while (true) {
		memset(buff,0,MAXLINE);
		n = recv(_clientFileDescriptor, buff, 4, 0);

		n=SocketUtil::bytesToInt(buff);

		int port;

		char *ip=getClientIPandPort(_clientFileDescriptor,port);

		cout<<"本次接收字节大小: "<<n<<" byte "<<" 请求地址: "<<ip<<" 端口号: "<<port
				<<endl;

		recv(_clientFileDescriptor,buff,n,0);

		printf("%s\n", buff);

		char *s="Hello,you are connected!\n";
		s=SocketProtocol::transferLocalStreamToNetWorkStream(s,n);

		send(_clientFileDescriptor, s, n, 0);

		if (memcmp(buff, "exit", strlen("exit")) == 0) {
			break;
		}

	}

	shutdown(_serverFileDescriptor, 2);
	shutdown(_clientFileDescriptor, 2);
}

char* ServerCore::getClientIPandPort(int fileDescriptor,int &port)
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
