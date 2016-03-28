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
//	auto client=ServerCore::getInstance()->_doubleConnector.getClient();

	auto hostRecThread=new Thread(receiveThread);

	hostRecThread->setArg(host);
	if(thread->getTag()=="host")
		hostRecThread->setTag("host");
	else
		hostRecThread->setTag("client");
	hostRecThread->start();
	hostRecThread->join();

//	auto clientRecThread=new Thread(receiveThread);
//
//	clientRecThread->setArg(client);
//	clientRecThread->setTag("client");
//	clientRecThread->start();
//	clientRecThread->join();



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

	cout<<thread->getTag()<<"方断开连接!"<<endl;
	(*ServerCore::getInstance()->_ofstream)<<thread->getTag()<<"方断开连接!"<<endl;
	SAFE_RELEASE(thread);
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
	string tag=th->getTag();
	if(memcmp(th->getTag().c_str(),"host",strlen("host"))==0)
	{
		target=ServerCore::getInstance()->_doubleConnector.getClient();
	}
	else
	{
		target=ServerCore::getInstance()->_doubleConnector.getHost();
	}
	string ip;
	if(target!=NULL)
		ip=target->getIP();

	while(true)
	{
		int len;
		char *content;
		content=SocketProtocol::transferNetWorkStreamToLocalStream(sock,len);
		if(len<=0)
		{
			(*ServerCore::getInstance()->_ofstream)<<"连接断开 "<<th->getTag()<<" "
					<<sock->getIP()<<endl;
			cout<<"连接断开"<<th->getTag()<<endl;
			SAFE_RELEASE(content);
			if(tag=="host")
			{
				ServerCore::getInstance()->_doubleConnector.setHost(NULL);
			}else if(tag=="client")
			{
				ServerCore::getInstance()->_doubleConnector.setClient(NULL);
			}
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

		if(memcmp(th->getTag().c_str(),"host",strlen("host"))==0)
		{
			target=ServerCore::getInstance()->_doubleConnector.getClient();
		}
		else
		{
			target=ServerCore::getInstance()->_doubleConnector.getHost();
		}

		if(target==NULL || len<=0 || !SocketProtocol::sendNetWorkStream(target,msg,len))
		{
			(*ServerCore::getInstance()->_ofstream)<<"连接断开"<<ip<<endl;
			cout<<"连接断开"<<ip<<endl;
			SAFE_RELEASE(content);
			SAFE_RELEASE(temp);
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
	if(_serverSocket==NULL)
	{
		_serverSocket = new ServerSocket();

		_serverSocket->bind("23.83.238.180",8000);
//		_serverSocket->bind("127.0.0.1",8000);
		_serverSocket->listen(10);

		_ofstream=new ofstream("log.lg");
	}

	while(true)
	{
		auto sock=_serverSocket->accept();

		cout<<"ip "<<sock->getIP()<<" port "<<sock->getPort()<<endl;

		_doubleConnector.setHost(sock);

		cout<<"宿主已连接! "<<"ip "<<sock->getIP()<<endl;
		(*ServerCore::getInstance()->_ofstream)<<"宿主已连接! "<<"ip "<<sock->getIP()<<endl;


		auto thread=new Thread();

		thread->setRunnable(threadRunnable);

		thread->setArg(sock);

		thread->setTag("host");

		thread->start();



		auto client=_serverSocket->accept();

		cout<<"ip "<<sock->getIP()<<" port "<<sock->getPort()<<endl;

		if(_doubleConnector.getHost()==NULL)
		{
			cout<<"没有宿主断开客户连接."<<endl;
			(*ServerCore::getInstance()->_ofstream)<<"没有宿主断开客户连接."<<endl;
			client->close();
			SAFE_RELEASE(client);
			ServerCore::run();
			return;
		}

		_doubleConnector.setClient(client);

		cout<<"客户已连接! ip "<<client->getIP()<<endl;
		(*ServerCore::getInstance()->_ofstream)<<"客户已连接! "<<"ip "<<sock->getIP()<<endl;

		auto thread1=new Thread();

		thread1->setRunnable(threadRunnable);

		thread1->setArg(client);

		thread1->setTag("client");

		thread1->start();

	}
	(*ServerCore::getInstance()->_ofstream).close();
}

char* ServerCore::getCurrentTime()
{
		time_t timep;
		time(&timep);
		return asctime(localtime(&timep));
}

