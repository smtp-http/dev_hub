#ifndef __CDYCONNBASEIMP_H__
#define __CDYCONNBASEIMP_H__

#include <event2/util.h>
#include <event2/event.h>
#include <event2/buffer.h>
#include "connbase.h"
using namespace std;

namespace lux{

class ConnectorImp
{
	string m_peerAddr;
	short m_peerPort;
	IConnectorAcceptorSink *m_sink;
	
	evutil_socket_t m_fd;
	struct event_base *m_evBase;
	struct event *m_connectEvent;
	Connector *m_wrapper;
	bool m_connecting;
public:
	ConnectorImp(const string &host, short port, IConnectorAcceptorSink *sink, Connector *wrapper);
	~ConnectorImp();
	
	//@param timeout means the connect timeout value in millesecond.
	int Connect(long timeout);
	
	void Cancel();
	
	void OnConnectResult(evutil_socket_t sock, int err, string &peerAddr, short peerPort);
};

class AcceptorImp
{
	string m_addr;
	short m_port;
	IConnectorAcceptorSink *m_sink;
	
	evutil_socket_t m_fd;
	struct event_base *m_evBase;
	struct event *m_connectEvent;
	Acceptor *m_wrapper;
public:
	AcceptorImp(const std::string &addr, short port, IConnectorAcceptorSink *sink, Acceptor *wrapper);
	
	int StartListen();
	
	void Stop();
	
	void OnConnectResult(evutil_socket_t sock, int err, string &peerAddr, short peerPort);
};

class UnixAcceptorImp
{
		string m_unixSock;
		IConnectorAcceptorSink *m_sink;
		
		evutil_socket_t m_fd;
		struct event_base *m_evBase;
		struct event *m_connectEvent;
		UnixAcceptor *m_wrapper;
	public:
		UnixAcceptorImp(const string unixSocket,IConnectorAcceptorSink *sink,UnixAcceptor *wrapper);
		
		int StartListen();
		
		void Stop();
		
		void OnConnectResult(evutil_socket_t sock);

};


class TcpConnection : public Connection
{
public:
	TcpConnection(evutil_socket_t fd, struct event_base *evBase);
	~TcpConnection();
	
	//virtual void SetConnectionSink(IConnectionSink *sink) { this->m_sink = sink; }
	
	//virtual int Send(const char *data, int len);
	
	//virtual void Close();

	const std::string& GetPeerAddress();

	short GetPeerPort();

	//void OnRead(evutil_socket_t sock);
	
	//void OnWrite(evutil_socket_t sock);

	//virtual void SetPeerAddress(const string addr);

	void SetPeerPort(short port);
	virtual bool GetPeerAddr(string &addr){addr = m_peerAddr;return true;}
protected:

	//string m_peerAddr;
	short m_peerPort;

};


class UnixConnection : public Connection
{
public:
	UnixConnection(evutil_socket_t fd, struct event_base *evBase);
	virtual ~UnixConnection(){}
	
	//virtual void SetConnectionSink(IConnectionSink *sink);
	
	//virtual int Send(const char *data, int len);
	
	//virtual void Close();

};


};
#endif //__CDYCONNBASEIMP_H__

