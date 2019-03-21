#include <string.h>
#include "UnixSockServer.h"

using namespace std;
using namespace lux;



UnixSockServer::UnixSockServer(const string unixSock)
//	: m_acceptor(new UnixAcceptor(unixSock, this))
{
	m_acceptor = new UnixAcceptor(unixSock, this);
}

UnixSockServer::~UnixSockServer()
{
	m_acceptor->Stop();
	delete m_acceptor;
	m_acceptor = NULL;
}

bool UnixSockServer::Start()
{
	return m_acceptor->StartListen() == 0;
}



////////////////////////////// test ///////////////////////////

UnixSockServer *UnixSockServerTest()
{
	UnixSockServer *server = new UnixSockServer("/tmp/go.sock");
		if (!server->Start()){
			cerr << "server start failed." << endl;
			return NULL;
		}
		cout << "server start ok" << endl;
		return server;
}






