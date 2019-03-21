#ifndef __COMM_SERVER_H__
#define __COMM_SERVER_H__

#include <list>
#include <map>
#include "connbase.h"
#include "session.h"


class CommServer : public lux::IConnectorAcceptorSink,public lux::IUpdateSink,public lux::ITransUpdate
{

public:
	CommServer();
	virtual void OnConnection(lux::Connection *conn, lux::IConnectorAcceptor *ca);
	virtual void UpdateInfo(const char *buf,int len);
	virtual void Update(const std::string data,unsigned int transId);
	
	//bool UnSubscibeTranscationId(int id,lux::Connection *conn);
	
	void SetDownChannel(lux::IDownChannel *dc);
	bool OnFrame(std::string &frame,lux::Session *session);

	void OnNewSession(lux::Session &session);
	void OnSessionClose(lux::Session &session);
	bool RemoveSessionInMap(lux::Session *s);
	virtual bool Start()= 0;
	void SetTransManager(lux::ITranscationOwner *tm);
protected:
	//lux::IConnectorAcceptor *m_acceptor;
	bool SubscibeTranscationId(int id,lux::Session *s);
	typedef std::list<lux::Connection*> ConnectionList;
	typedef std::list<lux::Session*> SessionList;
	typedef std::map<unsigned int,SessionList*> TranscationSessions;
	ConnectionList m_connections;
	TranscationSessions m_transcationSessions;
	lux::IDownChannel *m_downChannel;

	std::list<lux::Session*> m_sessionList;
	lux::VirtualAcceptor *m_acceptor;
	lux::ITranscationOwner *m_transManager;
	
};




#endif

