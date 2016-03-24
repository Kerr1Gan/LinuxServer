/*
 * SocketProtocol.h
 *
 *  Created on: 2016年3月21日
 *      Author: xiang
 */

#ifndef SRC_SOCKETUTILS_SOCKETPROTOCOL_H_
#define SRC_SOCKETUTILS_SOCKETPROTOCOL_H_
#include <iostream>
#include <stdlib.h>
#include <string>
#include <SocketUtil.h>
using namespace std;


class SocketProtocol {
public:
	SocketProtocol();
	virtual ~SocketProtocol();


	static char* transferLocalStreamToNetWorkStream(char* localStream,int &length);


	static char* transferNetWorkStreamToLocalStream(char* networkStream,int length);

	static int getNetWorkStreamLength(char *netWorkStream);

	static int getNetWorkStreamLengthWidthHead(char *netWorkStream);
};

#endif /* SRC_SOCKETUTILS_SOCKETPROTOCOL_H_ */
