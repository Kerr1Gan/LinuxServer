/*
 * ServerCore.cpp
 *
 *  Created on: 2016年3月22日
 *      Author: xiang
 */

#include <Server/ServerCore.h>
#include "Macro.h"
#include "Thread/Thread.h"
#include <signal.h>
ServerCore::ServerCore() {
	// TODO Auto-generated constructor stub

	_threadVec=new vector<Thread>;

	memset(&_doubleConnector,0,sizeof(_doubleConnector));
	_serverSocket=NULL;
	_ofstream=NULL;
	_errorTimeCount=0;
}

ServerCore::~ServerCore() {
	// TODO Auto-generated destructor stub
	_serverSocket->close();
	SAFE_RELEASE(_serverSocket);
	SAFE_RELEASE(_threadVec);
	SAFE_RELEASE(_ofstream);
}

void signal_catch(int signo);
void SetupSignal(void)
{
	sigset_t signal_mask;
	sigemptyset (&signal_mask);
	sigaddset (&signal_mask, SIGPIPE);
	int rc = pthread_sigmask (SIG_BLOCK, &signal_mask, NULL);
	if (rc != 0) {
	printf("block sigpipe error\n");
	}
//  struct sigaction act;
//
//  act.sa_handler = signal_catch;
//  act.sa_flags   = 0;
//  sigemptyset(&act.sa_mask);
//  sigaction(SIGHUP,&act,NULL);
//  sigaction(SIGINT,&act,NULL);
//  sigaction(SIGQUIT,&act,NULL);
//  sigaction(SIGILL,&act,NULL);
//  sigaction(SIGABRT,&act,NULL);
//  sigaction(SIGIOT,&act,NULL);
//  sigaction(SIGBUS,&act,NULL);
//  sigaction(SIGFPE,&act,NULL);
//  sigaction(SIGTERM,&act,NULL);
}

void signal_catch(int signo)
{
	cout<<"Broken Pipe "<<ServerCore::getInstance()->getCurrentTime()<<endl;
//  exit(-1);
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

	SetupSignal();
	auto hostRecThread=new Thread(receiveThread);

	hostRecThread->setArg(host);
	if(thread->getTag()=="host")
		hostRecThread->setTag("host");
	else if(thread->getTag()=="client")
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
	if(thread->getTag()=="host")
		ServerCore::getInstance()->_doubleConnector.setHost(NULL);
	else if(thread->getTag()=="client")
		ServerCore::getInstance()->_doubleConnector.setClient(NULL);

	cout<<thread->getTag()<<"方断开连接!"<<endl;

	(*ServerCore::getInstance()->_ofstream)<<thread->getTag()<<"方断开连接!"<<endl;

	if(ServerCore::getInstance()->_doubleConnector.getHost()==NULL
			&&ServerCore::getInstance()->_doubleConnector.getClient()==NULL)
		cout<<"当前连接没有任何连接"<<endl;
	SAFE_RELEASE(thread);
	return NULL;
}

void* sendThread(void* arg)
{
	auto th=(Thread*)arg;
	auto sock=th->getArg();



	return NULL;
}

Mutex _mutex;
void* receiveThread(void* arg)
{
	auto th=(Thread*)arg;
	auto sock=(Socket*)th->getArg();
	Socket* target;
	string tag=th->getTag();
	TimerThread* timer=NULL;
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
		char *content=NULL;

		content=SocketProtocol::readNetWorkStreamToLocalStream(sock,len);
		if(len<=0)
		{
			if(len==-2)
			{
				char *safeContent=new char[1024*1024];
				memset(safeContent,0,1024*1024);
				memcpy(safeContent,content,4);
				(*ServerCore::getInstance()->_ofstream)<<th->getTag()
										<<"收到有毒IP报文内容 "
										<<ServerCore::getInstance()->getCurrentTime()<<endl;
				int count=0;
				int size=0;

				timer = new TimerThread([&](void* arg)->void*
				{
					auto thread=(TimerThread*)arg;
					auto sock=(Socket*)thread->getArg();
					sleep(5);
					cout<<"Http强制关闭"<<endl;
					_mutex.lock();
					if(!thread->_isClose)
					{
						thread->_isClose=true;
						cout<<"release socket force"<<endl;
						sock->close();
//						SAFE_RELEASE(sock);
					}
//					SAFE_RELEASE(thread);
					_mutex.unLock();
					return NULL;
				});
				timer->setArg(sock);
				timer->start();
				while(true)
				{
					if (size == 0)
					{
						count = sock->receive(safeContent + 4, 6);
						size+=count+4;
					}
					else
					{
						count=sock->receive(safeContent+size,10);
						size+=count;
					}
					if(count<=0)
						break;
				}

				(*ServerCore::getInstance()->_ofstream)<<safeContent;
				SAFE_RELEASE_ARRAY(safeContent);
			}
			else
				SAFE_RELEASE_ARRAY(content);

			(*ServerCore::getInstance()->_ofstream)<<"连接断开 "<<th->getTag()<<" "
					<<sock->getIP()<<endl;
			cout<<"连接断开"<<th->getTag()<<endl;
			if(tag=="host")
			{
				ServerCore::getInstance()->_doubleConnector.setHost(NULL);
			}else if(tag=="client")
			{
				ServerCore::getInstance()->_doubleConnector.setClient(NULL);
			}
			break;
		}
		ServerCore::getInstance()->_errorTimeCount=0;

		if(DEBUG)
		cout<<tag<<" 收到"<<sock->getIP()<<" "<<len<<"字节  ";
		if(DEBUG)
		(*ServerCore::getInstance()->_ofstream)<<tag<<" 收到"<<sock->getIP()<<" "<<len<<"字节  ";


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
			if (tag == "host") {
				ServerCore::getInstance()->_doubleConnector.setClient(NULL);
				cout<<"client is empty."<<endl;
			} else if (tag == "client") {
				ServerCore::getInstance()->_doubleConnector.setHost(NULL);
				cout<<"host is empty."<<endl;
			}
			SAFE_RELEASE_ARRAY(content);
			SAFE_RELEASE_ARRAY(temp);
			SAFE_RELEASE_ARRAY(msg);
			continue;
		}
		ServerCore::getInstance()->_errorTimeCount=0;
		if(DEBUG)
		cout<<tag<<" 发送给"<<target->getIP()<<" "<<len<<"字节"<<endl;
		if(DEBUG && DEEPDEBUG)
		(*ServerCore::getInstance()->_ofstream)<<tag<<" 发送给"
				<<target->getIP()<<" "<<len<<"字节"<<"  内容:"<<temp<<endl;

		SAFE_RELEASE_ARRAY(temp);
		SAFE_RELEASE_ARRAY(msg);
		SAFE_RELEASE_ARRAY(content);
	}

	_mutex.lock();
	if(timer==NULL)
	{
		sock->close();
	}
	else if(!timer->_isClose)
	{
		cout<<"release socket normal"<<endl;
		timer->_isClose=true;
		if(timer!=NULL)
		{
			timer->cancle();
		}
		sock->close();
	}
	SAFE_RELEASE(sock);
	SAFE_RELEASE(th);
	_mutex.unLock();
	if(tag=="host")
	{
		ServerCore::getInstance()->_doubleConnector.setHost(NULL);
		cout<<"host is empty."<<endl;
	}
	else if(tag=="client")
	{
		ServerCore::getInstance()->_doubleConnector.setClient(NULL);
		cout<<"client is empty."<<endl;
	}

	return NULL;
}


void ServerCore::run()
{
	if(_serverSocket==NULL)
	{
		_serverSocket = new ServerSocket();

//		_serverSocket->bind("23.83.238.180",8000);
		_serverSocket->bind("127.0.0.1",8000);
		_serverSocket->listen(10);

		_ofstream=new ofstream("log.lg");
	}

	while(true)
	{
		if(_doubleConnector.getClient()!=NULL &&
				_doubleConnector.getHost()!=NULL)
		{
			(*ServerCore::getInstance()->_ofstream)<<
					"Queue is full sleep one seconds."<<endl;
			if(_errorTimeCount>=60)
			{
				cout<<"双方连接超时强行关闭."<<endl;

				(*_ofstream)<<"双方连接超时强行关闭."<<endl;

				_doubleConnector.getHost()->close();
				_doubleConnector.getClient()->close();
				_errorTimeCount=0;
			}
			sleep(1);
			_errorTimeCount+=1;
			continue;
		}
		Socket* sock=NULL;
		if (_doubleConnector.getHost() == NULL)
		{
			sock = _serverSocket->accept();
//			sock->setReciveTimeOut(60);

			cout << "ip " << sock->getIP() << " port " << sock->getPort()
					<< endl;

			_doubleConnector.setHost(sock);

			cout << "宿主已连接! " << "ip " << sock->getIP() << endl;
			(*ServerCore::getInstance()->_ofstream) << "宿主已连接! " << "ip "
					<< sock->getIP() << endl;

			SetupSignal();
			auto thread = new Thread();

			thread->setRunnable(threadRunnable);

			thread->setArg(sock);

			thread->setTag("host");

			thread->start();
		}
		else
		{
			sock=_doubleConnector.getHost();
		}

		Socket* client=NULL;

		if(_doubleConnector.getClient()==NULL)
		{
			client = _serverSocket->accept();
//			client->setReciveTimeOut(60);

			cout << "ip " << client->getIP() << " port " << sock->getPort()
					<< endl;
				if(_doubleConnector.getHost()==NULL)
				{
					cout<<"没有宿主,客户转为宿主."<<endl;
					(*ServerCore::getInstance()->_ofstream)<<"没有宿主,客户转为宿主."<<endl;

					_doubleConnector.setHost(client);
					cout << "宿主已连接! " << "ip " << client->getIP() << endl;
					(*ServerCore::getInstance()->_ofstream) << "宿主已连接! " << "ip "
							<< client->getIP() << endl;

					SetupSignal();
					auto thread = new Thread();

					thread->setRunnable(threadRunnable);

					thread->setArg(client);

					thread->setTag("host");

					thread->start();

					ServerCore::run();
					return;
				}

				_doubleConnector.setClient(client);

				cout<<"客户已连接! ip "<<client->getIP()<<endl;
				(*ServerCore::getInstance()->_ofstream)<<"客户已连接! "<<"ip "
						<<sock->getIP()<<endl;

				SetupSignal();
				auto thread1=new Thread();

				thread1->setRunnable(threadRunnable);

				thread1->setArg(client);

				thread1->setTag("client");

				thread1->start();
		}

	}
	(*ServerCore::getInstance()->_ofstream).close();
}

char* ServerCore::getCurrentTime()
{
		time_t timep;
		time(&timep);
		return asctime(localtime(&timep));
}

ServerSocket* ServerCore::getServerSocket()
{
	return _serverSocket;
}
