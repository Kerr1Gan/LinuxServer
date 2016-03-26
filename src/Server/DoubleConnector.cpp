/*
 * DoubleConnector.cpp
 *
 *  Created on: 2016年3月25日
 *      Author: xiang
 */

#include <Server/DoubleConnector.h>

DoubleConnector::DoubleConnector() {
	// TODO Auto-generated constructor stub
	 _host=NULL;
	 _client=NULL;
}

DoubleConnector::~DoubleConnector() {
	// TODO Auto-generated destructor stub
	SAFE_RELEASE(_host);
	SAFE_RELEASE(_client);
}

Socket* DoubleConnector::getHost()
{
	return _host;
}

Socket* DoubleConnector::getClient()
{
	return _client;
}

