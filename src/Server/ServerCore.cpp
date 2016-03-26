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

	_threadVec=new vector<Thread>;

	memset(&_doubleConnector,0,sizeof(_doubleConnector));
	_serverSocket=NULL;
	_ofstream=NULL;
}

ServerCore::~ServerCore() {
	// TODO Auto-generated destructor stub
	_serverSocket->close();
	SAFE_RELEASE(_serverSocket);
	SAFE_RELEASE(_threadVec);
	SAFE_RELEASE(_ofstream);
}

ServerCore* ServerCore::getInstance()
{
		if(_serverCoreInstance==NULL)
			_serverCoreInstance=new ServerCore();

		return _serverCoreInstance;
}

void* sendThread(void* arg);

void* receiveThread(void* arg);


void*  threadRunnable (void * arg)
{
	auto thread=(Thread*)arg;
	auto host=(Socket*)thread->getArg();
	auto client=ServerCore::getInstance()->_doubleConnector.getClient();

	auto hostRecThread=new Thread(receiveThread);

	hostRecThread->setArg(host);
	hostRecThread->setTag("host");
	hostRecThread->start();


	auto clientRecThread=new Thread(receiveThread);

	clientRecThread->setArg(client);
	clientRecThread->setTag("client");
	clientRecThread->start();
	clientRecThread->join();



//	char bytes[5];
//	while(true)
//	{
//		memset(bytes,0,5);
//
//		host->receive(bytes,4);
//		int len=0;
//		len=SocketUtil::bytesToInt(bytes);
//
//		char* content=new char[len+1];
//		memset(content,0,len+1);
//		host->receive(content,len);
//
//		if(len<=0)
//		{
//				cout<<"已断开连接！"<<endl;
//				break;
//		}
//		cout<<"接收字节数 "<<len<<" 内容 "<<content<<endl;
//
//		char* transMsg=SocketProtocol::transferLocalStreamToNetWorkStream(content,len);
//
//		len=client->send(transMsg,len);
//
//		if(len<=0)
//			break;
//
//		cout<<"转发给IP "<<client->getIP()<<" "<<len<<"字节"<<endl;
//
//		SAFE_RELEASE(transMsg);
//		SAFE_RELEASE(content);
//	}


	SAFE_RELEASE(thread);
	cout<<"双方均断开连接!"<<endl;
	(*ServerCore::getInstance()->_ofstream)<<"双方均断开连接！"<<endl;
	return NULL;
}

void* sendThread(void* arg)
{
	auto th=(Thread*)arg;
	auto sock=th->getArg();



	return NULL;
}

void* receiveThread(void* arg)
{
	auto th=(Thread*)arg;
	auto sock=(Socket*)th->getArg();
	Socket* target;
	if(memcmp(th->getTag().c_str(),"host",strlen("host"))==0)
	{
		target=ServerCore::getInstance()->_doubleConnector.getClient();
	}
	else
	{
		target=ServerCore::getInstance()->_doubleConnector.getHost();
	}

	while(true)
	{
		int len;
		char *content;
		content=SocketProtocol::transferNetWorkStreamToLocalStream(sock,len);
		if(len<=0)
		{
			(*ServerCore::getInstance()->_ofstream)<<"连接断开 "<<th->getTag()<<" "
					<<sock->getIP()<<endl;
			cout<<"连接断开"<<endl;
			SAFE_RELEASE(content);
			break;
		}

		cout<<"收到"<<sock->getIP()<<" "<<len<<"字节  ";
		(*ServerCore::getInstance()->_ofstream)<<"收到"<<sock->getIP()<<" "<<len<<"字节  ";


		char* temp=new char[len+1];
		memset(temp,0,len+1);
		memcpy(temp,content,len);
		char* msg;
		msg=SocketProtocol::transferLocalStreamToNetWorkStream(temp,len);
//		target->send(msg,len);
		SocketProtocol::sendNetWorkStream(target,msg,len);
		if(len<=0)
		{
			(*ServerCore::getInstance()->_ofstream)<<"连接断开"<<target->getIP()<<endl;
			cout<<"连接断开"<<endl;
			SAFE_RELEASE(content);
			SAFE_RELEASE(msg);
			break;
		}
		cout<<"发送给"<<target->getIP()<<" "<<len<<"字节"<<endl;
		(*ServerCore::getInstance()->_ofstream)<<"发送给"
				<<target->getIP()<<" "<<len<<"字节"<<"  内容:"<<temp<<endl;

		SAFE_RELEASE(temp);
		SAFE_RELEASE(msg);
		SAFE_RELEASE(content);
	}

	sock->close();
	SAFE_RELEASE(sock);
	SAFE_RELEASE(th);
	return NULL;
}


void ServerCore::run()
{
	_serverSocket = new ServerSocket();

	_serverSocket->bind("23.83.238.180",8000);
//	_serverSocket->bind("127.0.0.1",8000);
	_serverSocket->listen(10);

	_ofstream=new ofstream("log.lg");


	while(true)
	{
		auto sock=_serverSocket->accept();

		cout<<"ip "<<sock->getIP()<<" port "<<sock->getPort()<<endl;

		_doubleConnector.setHost(sock);

		cout<<"宿主已连接! "<<"ip "<<sock->getIP()<<endl;
		(*ServerCore::getInstance()->_ofstream)<<"宿主已连接! "<<"ip "<<sock->getIP()<<endl;
		auto client=_serverSocket->accept();

		cout<<"ip "<<sock->getIP()<<" port "<<sock->getPort()<<endl;

		_doubleConnector.setClient(client);

		cout<<"客户已连接! ip "<<client->getIP()<<endl;
		(*ServerCore::getInstance()->_ofstream)<<"宿主已连接! "<<"ip "<<sock->getIP()<<endl;
		auto thread=new Thread();

		thread->setRunnable(threadRunnable);

		thread->setArg(sock);

		thread->start();


//		auto thread2=new Thread();
//
//		thread2->_bindRunnabe=([=](void* arg)->void*
//		{
//
//		});
//
//		thread2->setArg(client);
//
//		thread2->start();
	}
	(*ServerCore::getInstance()->_ofstream).close();
}

