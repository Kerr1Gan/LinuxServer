/*
 * ServerCore.cpp
 *
 *  Created on: 2016年3月22日
 *      Author: xiang
 */

#include <Server/ServerCore.h>
#include "Macro.h"
#include "Thread/Thread.h"
ServerCore::ServerCore() {
	// TODO Auto-generated constructor stub
	_clientFileDescriptor=-1;
	_serverSocket=NULL;
}

ServerCore::~ServerCore() {
	// TODO Auto-generated destructor stub
	_serverSocket->close();
	SAFE_RELEASE(_serverSocket);
}

ServerCore* ServerCore::getInstance()
{
		if(_serverCoreInstance==NULL)
			_serverCoreInstance=new ServerCore();

		return _serverCoreInstance;
}


void*  threadRunnable (void * arg)
{
	auto thread=(Thread*)arg;
	auto client=(Socket*)thread->getArg();
	char bytes[5];

	while(true)
	{
		memset(bytes,0,5);

		client->receive(bytes,4);
		int len=0;
		len=SocketUtil::bytesToInt(bytes);

		char* content=new char[len+1];
		memset(content,0,len+1);
		cout<<len<<endl;
		client->receive(content,len);

		if(len<=0)
		{
				cout<<"已断开连接！"<<endl;
				break;
		}
		cout<<"接收字节数 "<<len<<" 内容 "<<content<<endl;

		SAFE_RELEASE(content);
	}

	client->close();
	SAFE_RELEASE(thread);
	return NULL;
}


void ServerCore::run()
{
	_serverSocket = new ServerSocket();

	_serverSocket->bind("127.0.0.1",8000);

	_serverSocket->listen(10);


	while(true)
	{
		auto sock=_serverSocket->accept();

		cout<<"ip "<<sock->getIP()<<" port "<<sock->getPort()<<endl;

		auto thread=new Thread();

		thread->setRunnable(threadRunnable);

		thread->setArg(sock);

		thread->run();
	}

}

