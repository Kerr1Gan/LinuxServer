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
	length=strlen(localStream);

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

/*
 * 	return netWorkStream full length,except \0
 *
 * */
int SocketProtocol::getNetWorkStreamLengthWidthHead(char *netWorkStream)
{
		return getNetWorkStreamLength(netWorkStream)+4;
}


