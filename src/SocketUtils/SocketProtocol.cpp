/*
 * SocketProtocol.cpp
 *
 *  Created on: 2016年3月21日
 *      Author: xiang
 */

#include "SocketProtocol.h"

SocketProtocol::SocketProtocol() {
	// TODO Auto-generated constructor stub
}

SocketProtocol::~SocketProtocol() {
	// TODO Auto-generated destructor stub
}

/*
 * 		must be release char ptr.not contains \0
*/
char* SocketProtocol::transferLocalStreamToNetWorkStream(char* localStream,int &length)
{
//	length=strlen(localStream);

	char bytes[5];
	memset(bytes,0,5);
	SocketUtil::intToBytes(length,bytes);

//	string head;
//
//	head.assign((char*)bytes,0,4);

	char *result=new char[length+4];

	result[0]=bytes[0];
	result[1]=bytes[1];
	result[2]=bytes[2];
	result[3]=bytes[3];

	for(int i=4;i<length+4;i++)
	{
		result[i]=localStream[i-4];
	}

	length+=4;
	return result;
}

/*
 * 	must be release char ptr.
*/
char* SocketProtocol::transferNetWorkStreamToLocalStream(char* networkStream
		,int length)
{
	char *result=new char[length-4];
	for(int i=4;i<length;i++)
	{
		result[i-4]=networkStream[i];
	}
	return result;
}

/*
 * 	must be release char ptr.
*/
char* SocketProtocol::readNetWorkStreamToLocalStream(Socket* socket,int &length)
{
	if(socket==NULL	)
		return NULL;

	char heads[5];
	memset(heads,0,5);

	if(socket->receive(heads,4)<=0)
	{
		length=-1;
		return NULL;
	}
	int len=SocketUtil::bytesToInt(heads);
	if(len<0 || len>10*1024*1024)
	{
		cout<<"报文头部错误，可能是有毒IP入侵。"<<socket->getIP()<<endl;
		length=-2;
		return heads;
	}

	char* content=new char[len];
	memset(content,0,len);
	int size=0;
	int count=0;
	while(size<len)
	{
		if(len-size<_defaultMaxReceiveBytes)
		{
			count=socket->receive(content+size,len-size);
			if(count<=0)
			{
				length=-1;
				return NULL;
			}
			size+=count;
			continue;
		}
		count=socket->receive(content+size,_defaultMaxReceiveBytes);
		if(count<=0)
		{
			length=-1;
			return NULL;
		}
		size+=count;
	}
	length=len;
	return content;
}


/*
 * 	return netWorkStream content length,except \0
 *
 * */
int SocketProtocol::getNetWorkStreamLength(char *netWorkStream)
{
		char heads[5];
		memset(heads,0,5);

		heads[0]=netWorkStream[0];
		heads[1]=netWorkStream[1];
		heads[2]=netWorkStream[2];
		heads[3]=netWorkStream[3];

		return SocketUtil::bytesToInt(heads);
}


bool SocketProtocol::sendNetWorkStream(Socket* socket,char* content,int &len)
{
	int size=0;
	int count=0;
	signal(SIGPIPE, SIG_IGN);
	while(size<len)
	{
		if(len-size<_defaultMaxSendBytes)
		{
			count=socket->send(content+size,len-size);
			if (count <= 0)
			{
				len=-1;
				return false;
			}
			size+=count;
			continue;
		}
		count=socket->send(content+size,_defaultMaxSendBytes);
		if (count <= 0)
		{
			len=-1;
			return false;
		}
		size+=count;
	}
	len=size;
	return true;
}


/*
 * 	return netWorkStream full length,except \0
 *
 * */
int SocketProtocol::getNetWorkStreamLengthWidthHead(char *netWorkStream)
{
		return getNetWorkStreamLength(netWorkStream)+4;
}

void SocketProtocol::setDefaultMaxReceiveBytes(int count)
{
	_defaultMaxReceiveBytes=count;
}

void SocketProtocol::setDefaultMaxSendBytes(int count)
{
	_defaultMaxSendBytes=count;
}

/*
 * need to release char ptr.
*/
char* SocketProtocol::receiveNetWorkStreamExactly(Socket* sock,int& length)
{
	int size=0;
	int count=0;
	char *content=new char[length];
	memset(content,0,length);
	while(size<length)
	{
		if(length-size<_defaultMaxReceiveBytes)
		{
			count=sock->receive(content+size,length-size);
			if(count<=0)
			{
				length=-1;
				return content;
			}
			size+=count;
			continue;
		}
		count=sock->receive(content+size,_defaultMaxReceiveBytes);
		if (count <= 0)
		{
			length = -1;
			return content;
		}
		size+=count;
	}
	return content;
}

int SocketProtocol::sendNetWorkStreamExactly(Socket* sock,char* content,int length)
{
	int size=0;
	int count=0;
	while(size<length)
	{
		if(length-size<_defaultMaxReceiveBytes)
		{
			count=sock->send(content+size,length-size);
			if(count<=0)
			{
				return size;
			}
			size+=count;
			continue;
		}
		count=sock->send(content+size,_defaultMaxSendBytes);
		if (count <= 0)
		{
			return size;
		}
		size+=count;
	}
	return size;
}
