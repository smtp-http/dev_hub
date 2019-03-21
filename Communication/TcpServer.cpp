#include <string>
#include <string.h>
#include <list>
#include <iostream>
#include <algorithm>
#include "connbase.h"
#include "eventlooper.h"
#include "TcpServer.h"

using namespace std;
using namespace lux;


TcpServer::TcpServer(const string &addr, short port)
//		: m_acceptor(new Acceptor(addr, port, this))
{
	m_acceptor = new Acceptor(addr, port, this);
}

TcpServer::~TcpServer()
{
	m_acceptor->Stop();
	delete m_acceptor;
	m_acceptor = NULL;
}

bool TcpServer::Start()
{
	return m_acceptor->StartListen() == 0;
}


////////////////////////////// test ///////////////////////////

TcpServer *TcpServerTest()
{
	TcpServer *server = new TcpServer("0.0.0.0", 5678);//"10.10.80.15"
		if (!server->Start()){
			cerr << "server start failed." << endl;
			return NULL;
		}
		
		return server;
}


