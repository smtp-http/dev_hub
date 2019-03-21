#ifndef __LUXCONNBASE_H__
#define __LUXCONNBASE_H__

#include <string>
#include <list>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/buffer.h>


#define GET_ARRAY_LEN(array,len){len = (sizeof(array) / sizeof(array[0]));}

#define ERROR_FORMAT  1
#define ERROR_PARAM   2
#define ERROR_COMMAND 3
#define ERROR_DEVOPEN 4


namespace lux{

class Connection;
class IConnectorAcceptor;

enum TaskStatus {
	task_idle = 0,
	task_accept,
	task_run
};

enum SequenceType {
	GlueSequence = 0,
	SolderSequence,
};


enum CommunicationFormat {
	format_json = 0,
	format_common
};


//User should implemention this interface to get the new connection notify.
class IConnectorAcceptorSink
{
public:
	//@param conn is the new connection if succeed, else null.
	//@ca shows the Connector or Acceptor object which created the new connection.
	virtual void OnConnection(Connection *conn, IConnectorAcceptor *ca) = 0;
};

//User should implemention this interface to get the notification of data incoming
//or the buffer is not full so user can write more data, and the event of connection
//been disconnected.
class IConnectionSink
{
public:
	// called when new data income.
	virtual void OnData(const char *buf, int length, Connection *conn) = 0;
	// called when user can write data to network.
	virtual void OnWrite(Connection *conn) = 0;
	// called when the connection is broken, if user call Connection::Close(), 
	// will not get this callback.
	virtual void OnDisconnect(int reason, Connection *conn) = 0;
};

class IMsgSender
{
public:
	virtual int SendMessage(std::string str) = 0;
};

class IUpdateSink
{
public:
	virtual void UpdateInfo(const char *buf,int len) = 0;
	//virtual void UpdateInfo(const std::string data,unsigned int transId) = 0;
};


class IWritePeripheralData 
{
public:
	virtual int WriteData(const char *data,int len) = 0;
};


class ITransUpdate
{
public:
	virtual void Update(const std::string data,unsigned int transId) = 0;
};

class ITaskUse;

class ITranscationOwner
{
public:
	virtual void GetTransIds(std::list<int> &transIds,std::string ip) = 0;
	virtual void SetTransTask(unsigned int id,ITaskUse *task) = 0;
};


class IDownChannel 
{
public:
	virtual bool OnDownStream(unsigned int transId,std::string &frame) = 0;

};

class ITaskUse
{
public:
	virtual void SetStatus(lux::TaskStatus status) = 0;
	virtual TaskStatus GetTaskStatus() = 0; 
	virtual void AddTranscation(unsigned int tsId) = 0;
	virtual void Update(const std::string data,unsigned int transId) = 0;
	virtual void SetAuxiliaryParam(const std::string MachineId,const std::string stationId,const std::string slotId) = 0;
	virtual void SetCommFormat(CommunicationFormat f) = 0;
};


class IConnectorAcceptor
{
protected:
	int m_error;
public:
	IConnectorAcceptor() : m_error(0) {}
	virtual ~IConnectorAcceptor() {}
	
	virtual bool IsConnector() = 0;

	virtual int GetError() { return m_error; }
};

class Connector : public IConnectorAcceptor
{
public:
	Connector(const std::string &host, short port, IConnectorAcceptorSink *sink);

	~Connector();
	
	//@param timeout means the connect timeout value in millesecond.
	int Connect(long timeout);
	
	void Cancel();
	
	bool IsConnector() { return true; }
private:
	class ConnectorImp *m_imp;
};


class VirtualAcceptor : public IConnectorAcceptor
{
public:
	virtual int StartListen()= 0;
	virtual void Stop() = 0;
	bool IsConnector() { return false; }
};

class Acceptor : public VirtualAcceptor
{
public:
	Acceptor(const std::string &addr, short port, IConnectorAcceptorSink *sink);

	~Acceptor();
	
	virtual int StartListen();
	
	virtual void Stop();
	
	//bool IsConnector() { return false; }
private:
	class AcceptorImp *m_imp;
};

class UnixAcceptor : public VirtualAcceptor
{
	public:
		UnixAcceptor(const std::string unix_socket,IConnectorAcceptorSink *sink);
	
		~UnixAcceptor();
		
		virtual int StartListen();
		
		virtual void Stop();
		
		//virtual bool IsConnector() { return false; }
	private:
		class UnixAcceptorImp *m_imp;

};


class Connection
{
public:
	virtual ~Connection(){}
	
	void SetConnectionSink(IConnectionSink *sink){ this->m_sink = sink; }// = 0;
	
	//virtual int Send(const char *data, int len) = 0;
	int Send(const char *data, int len);
	
	//virtual void Close() = 0;
	void Close();
	void OnRead(evutil_socket_t sock);
	
	void OnWrite(evutil_socket_t sock);
	void SetPeerAddress(const std::string addr){m_peerAddr = addr;}
	virtual bool GetPeerAddr(std::string &addr){return false;}        // if unix connection,return false

	//virtual const std::string& GetPeerAddress() = 0;

	//virtual short GetPeerPort() = 0;
protected:
	IConnectionSink *m_sink;
	evutil_socket_t m_fd;
	struct event_base *m_evBase;
	struct event *m_readEvent;
	struct event *m_writeEvent;
	
	static const int MAX_READ_LEN = 2048;
	char m_readBuffer[2048];
	struct evbuffer *m_writeBuffer;
	bool m_closing;

	std::string m_peerAddr;
};

};
#endif //__LUXCONNBASE_H__

