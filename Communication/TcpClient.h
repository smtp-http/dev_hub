#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include <string>
//#include <list>
#include <iostream>
//#include <algorithm>
#include "connbase.h"
#include "eventlooper.h"

//using namespace std;
using namespace lux;

class TcpClient : public IConnectorAcceptorSink, public IConnectionSink, public ITimerUserSink
{
public:
	TcpClient(std::string peerAddr, short peerPort)
		: m_peerAddr(peerAddr)
		, m_peerPort(peerPort)
		, m_connector(NULL)
		, m_connection(NULL)
		, m_timerReconn(-1)
		, m_timerTest(-1)
	{
	}
	~TcpClient(){}

	bool Connect();
protected:
	void OnConnection(Connection *conn, IConnectorAcceptor *ca);

	void OnData(const char *buf, int length, Connection *conn);

	void OnWrite(Connection *conn);
	void OnDisconnect(int reason, Connection *conn);

	void OnTimer(TimerID tid);
private:
	std::string m_peerAddr;
	short m_peerPort;
	Connector *m_connector;
	Connection *m_connection;
	TimerID m_timerReconn;
	TimerID m_timerTest;
};



#endif
