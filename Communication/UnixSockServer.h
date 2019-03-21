#ifndef __UNIX_SOCK_SERVER_H__
#define __UNIX_SOCK_SERVER_H__

#include <string>
#include <list>
#include <iostream>
#include <algorithm>
#include <queue>
#include "connbase.h"
#include "eventlooper.h"
#include "CommServer.h"
//#include "TcpServer.h"

class UnixSockServer : public CommServer//lux::IConnectorAcceptorSink, public lux::IConnectionSink, public lux::IUpdateSink
{
	//lux::UnixAcceptor *m_acceptor;

public:
	UnixSockServer(const std::string unixSock);
	~UnixSockServer();
	virtual bool Start();
};

#endif

