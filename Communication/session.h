#ifndef __SESSION_H__
#define __SESSION_H__

#include <vector>
#include <string>
#include "connbase.h"
#include "eventlooper.h"

class CommServer;

namespace lux {

class Session : public lux::IConnectionSink,public IMsgSender
{
public:
	Session(CommServer *server);
	~Session();
	
	virtual int SendMessage(std::string str);
	virtual void Close();
	virtual void SetConnection(Connection *connection);
	
	virtual void OnData(const char *buf, int length, Connection *conn);
	// called when user can write data to network.
	virtual void OnWrite(Connection *conn);
	// called when the connection is broken, if user call Connection::Close(), 
	// will not get this callback.
	virtual void OnDisconnect(int reason, Connection *conn);

	ITaskUse *M_Task(){return m_task;}
	
protected:
	
	Connection *m_connection;
	std::vector<char> m_rdBuffer;
	TimerID m_timeout;
	CommServer *m_server;
	
private:
	ITaskUse *m_task;
	//std::string m_peerIp;
};

};


#endif

