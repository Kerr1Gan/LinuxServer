/*
 * DoubleConnector.h
 *
 *  Created on: 2016年3月25日
 *      Author: xiang
 */

#ifndef SRC_SERVER_DOUBLECONNECTOR_H_
#define SRC_SERVER_DOUBLECONNECTOR_H_
#include "SocketUtils/SocketHeads.h"
#include "Macro.h"
class DoubleConnector {
public:
	DoubleConnector();
	virtual ~DoubleConnector();

	void setHost(Socket *host);

	void setClient(Socket *client);

	Socket* getHost();

	Socket* getClient();

private:
	Socket* _host;
	Socket* _client;

};

#endif /* SRC_SERVER_DOUBLECONNECTOR_H_ */
