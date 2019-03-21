#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include <string>
#include <list>
#include <iostream>
#include <algorithm>
#include <queue>
#include "connbase.h"
#include "eventlooper.h"
#include "CommServer.h"

//using namespace lux;

class TcpServer : public CommServer
{
	//lux::Acceptor *m_acceptor;

public:
	TcpServer(const std::string &addr, short port);
	~TcpServer();
	virtual bool Start();
};


#endif

