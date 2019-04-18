#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include <string>
//#include <list>
#include <iostream>
//#include <algorithm>
#include "connbase.h"
#include "eventlooper.h"

using namespace std;
using namespace lux;

class TcpClient : public IConnectorAcceptorSink, public IConnectionSink, public ITimerUserSink
{
public:
	TcpClient(const string &peerAddr, short peerPort)
		: m_peerAddr(peerAddr)
		, m_peerPort(peerPort)
		, m_connector(NULL)
		, m_connection(NULL)
		, m_timerReconn(-1)
		, m_timerTest(-1)
	{
	}

	bool Connect();
protected:
	virtual void OnConnection(Connection *conn, IConnectorAcceptor *ca);

	virtual void OnData(const char *buf, int length, Connection *conn);

	virtual void OnWrite(Connection *conn);
	virtual void OnDisconnect(int reason, Connection *conn);

	virtual void OnTimer(TimerID tid);
private:
	string m_peerAddr;
	short m_peerPort;
	Connector *m_connector;
	Connection *m_connection;
	TimerID m_timerReconn;
	TimerID m_timerTest;
};



#endif
