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
#include <wait.h>
#include <signal.h>

void childProcessDeath(int signal)
{
	cout<<"child process death."<<endl;
	wait(&signal);
}


int main() {

	if(signal(SIGCHLD,childProcessDeath)==SIG_ERR)
		return -1;

	int childPID=fork();

	if(childPID==-1)
		perror("create child process:");
	else if(childPID==0)
	{
		cout<<"this is child process.pid "<<getpid()<<endl;
		int parentID=getppid();
		while(true)
		{
			if(getppid()!=parentID)
			{
				cout<<"父进程被杀死，尝试恢复."<<endl;
				if (execv("./Service", NULL) < 0)
					perror("Error on execv");
				return 0;
			}
			sleep(10);
		}
	}

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

	auto cinThread = new Thread([](void*)->void*
	{
		char command[20];
		memset(command,0,20);
		fgets(command,20,stdin);
		if(memcmp(command,"exit",strlen("exit"))==0)
		{
			ServerCore::getInstance()->getServerSocket()->close();
			exit(0);
		}
		return NULL;
	});
	cinThread->start();

	ServerCore::getInstance()->run();

	auto sPt=ServerCore::getInstance();
	SAFE_RELEASE(sPt);
	SAFE_RELEASE(timeT);
	SAFE_RELEASE(cinThread);
	return 0;

}




