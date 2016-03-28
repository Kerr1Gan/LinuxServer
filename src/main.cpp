#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <iostream>
using namespace std;
#include <SocketHeads.h>
#include <Server/ServerCore.h>
#include <pthread.h>
#include <unistd.h>
#include "Thread/Thread.h"
#include "Macro.h"
int main() {

	Thread* timeT = new Thread([](void*)->void*
	{
		while(true)
		{
			cout<<"Service:I'm alive. "<<ServerCore::getInstance()->getCurrentTime()<<endl;
			sleep(60*5);
		}
		return NULL;
	});

	timeT->start();

	ServerCore::getInstance()->run();

	auto sPt=ServerCore::getInstance();
	SAFE_RELEASE(sPt);
	return 0;

}




