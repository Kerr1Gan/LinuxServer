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
	 _isHostReady=false;
	 _isClientReady=false;
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

void DoubleConnector::setHost(Socket *host)
{
	_host=host;
}

void DoubleConnector::setClient(Socket *client)
{
	_client=client;
}

bool DoubleConnector::isHostReady()
{
	return _isHostReady;
}

bool DoubleConnector::isClientReady()
{
	return _isClientReady;
}

void DoubleConnector::hostReady(bool tmp)
{
	_isHostReady=tmp;
}

void DoubleConnector::clientReady(bool tmp)
{
	_isClientReady=tmp;
}
