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


int count=0;
pthread_mutex_t mutex;
pthread_cond_t cond;
void *threadHello(void *arg)
{
	printf("Hello Linux Thread!!\n");
	*(int*)arg=0;

	int i=*((int*)arg);


	for(int i=0;i<10000;i++)
	{
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond,&mutex);
		count++;
		cout<<count<<endl;
		pthread_mutex_unlock(&mutex);
	}

	return arg;
}

int main() {

	pthread_t thread;

	int ret;
	int arg=10;

	int *thread_ret=NULL;

	pthread_mutex_init(&mutex,NULL);
//	pthread_cond_init(&cond,NULL,NULL);

	ret=pthread_create(&thread,NULL,threadHello,&arg);

	if(ret!=0)
	{
		cout<<"Create Thread Error!!"<<endl;
		return -1;
	}


	cout<<"This is main process."<<endl;

//	pthread_join(thread,(void **)&thread_ret);


//	cout<<"thread_ret ="<<*thread_ret<<"."<<endl;


	for(int i=0;i<10000;i++)
	{
		pthread_mutex_lock(&mutex);
		count++;
		cout<<count<<endl;
		 pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
	}

	cout<<"end"<<endl;
	return 0;

//	ServerCore::getInstance()->run();

}

//		char* test="1234";
//
//		int len;
//		char* str1=SocketProtocol::transferLocalStreamToNetWorkStream(test,len);
//
//		int size=SocketProtocol::getNetWorkStreamLengthWidthHead(str1);
//
//		cout<<len<<endl;
//		cout<<size<<endl;
//		cout<<SocketProtocol::getNetWorkStreamLength(str1)<<endl;
//
//		char* str2=SocketProtocol::transferNetWorkStreamToLocalStream(str1,size);
//
//		cout<<str2<<endl;
//
//		SAFE_RELEASE(str1);
//		SAFE_RELEASE(str2);
