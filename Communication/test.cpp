#include <string>
#include <list>
#include <iostream>
#include <algorithm>
#include "connbase.h"
#include "eventlooper.h"


using namespace std;
using namespace lux;


class TestServer : public IConnectorAcceptorSink, public IConnectionSink
{
	Acceptor *m_acceptor;
	typedef list<Connection*> ConnectionList;
	ConnectionList m_connections;
public:
	TestServer(const string &addr, short port)
		: m_acceptor(new Acceptor(addr, port, this))
	{
	}

	~TestServer(){
		m_acceptor->Stop();
		delete m_acceptor;
		m_acceptor = NULL;
	}

	bool Start(){
		return m_acceptor->StartListen() == 0;
	}

protected:
	virtual void OnConnection(Connection *conn, IConnectorAcceptor *ca){
		if (conn){
			//cout << "Acceptor: new connection from " << conn->GetPeerAddress() << ":" << conn->GetPeerPort() << endl;
			conn->SetConnectionSink(this);
			m_connections.push_back(conn);
		}
	}

	virtual void OnData(const char *buf, int length, Connection *conn){
		cout << "OnData" << endl;
		string s(buf, length);
		cout << "recv: " << s << endl;
		conn->Send(buf, length);
	}

	virtual void OnWrite(Connection *conn){
		cout << "OnWrite" << endl;
	}

	virtual void OnDisconnect(int reason, Connection *conn)
	{
		//cout << "OnDisconnect from " << conn->GetPeerAddress() << ":" << conn->GetPeerPort() << endl;
		m_connections.erase(std::find(m_connections.begin(), m_connections.end(), conn));
		delete conn;
	}
};



class TestClient : public IConnectorAcceptorSink, public IConnectionSink, public ITimerUserSink
{
	string m_peerAddr;
	short m_peerPort;
	Connector *m_connector;
	Connection *m_connection;
	TimerID m_timerReconn;
	TimerID m_timerTest;

public:
	TestClient(const string &peerAddr, short peerPort)
		: m_peerAddr(peerAddr)
		, m_peerPort(peerPort)
		, m_connector(NULL)
		, m_connection(NULL)
		, m_timerReconn(-1)
		, m_timerTest(-1)
	{
	}

	bool Connect(){
		if (m_connector)
			return true;
		m_connector = new Connector(m_peerAddr, m_peerPort, this);
		return m_connector->Connect(3000) == 0; //3 seconds
	}
protected:
	virtual void OnConnection(Connection *conn, IConnectorAcceptor *ca){
		if (conn){
			//cout << "Client: connected succeed to " << conn->GetPeerAddress() << ":" << conn->GetPeerPort() << endl;
			m_connection = conn;

			string msg("hello, libevent!");
			conn->SetConnectionSink(this);
			//conn->Send(msg.c_str(), msg.length());
			struct timeval tv={0, 500*1000};
			m_timerTest = EventLooper::GetInstance().ScheduleTimer(&tv, TF_FIRE_PERIODICALLY, this);

			if (m_timerReconn != -1){
				EventLooper::GetInstance().CancelTimer(m_timerReconn);
				m_timerReconn = -1;
			}
		} else {
			cout << "Client: connect to " << m_peerAddr << ":" << m_peerPort << " failed." << endl;
		}
		delete m_connector;
		m_connector = NULL;

		if (!conn){
			cout << "not connected, try again!" << endl;
			//Connect();
			if (m_timerReconn == -1){
				struct timeval tv={1, 0};
				m_timerReconn = EventLooper::GetInstance().ScheduleTimer(&tv, TF_FIRE_PERIODICALLY, this);
			}
		}
	}

	virtual void OnData(const char *buf, int length, Connection *conn){
		cout << "OnData" << endl;
		string s(buf, length);
		cout << "recv: " << s << endl;
		//conn->Send(buf, length);
		//EventLooper::GetInstance().StopEventLoop(3000);
	}

	virtual void OnWrite(Connection *conn){
		cout << "OnWrite" << endl;
	}

	virtual void OnDisconnect(int reason, Connection *conn)
	{
		//cout << "OnDisconnect from " << conn->GetPeerAddress() << ":" << conn->GetPeerPort() << endl;
		delete m_connection;
		m_connection = NULL;

		EventLooper &el = EventLooper::GetInstance();
		if (m_timerTest != -1){
			el.CancelTimer(m_timerTest);
			m_timerTest = -1;
		}

		el.StopEventLoop(100);
	}

	virtual void OnTimer(TimerID tid)
	{
		//cout << "OnTimer " << tid << endl;
		if (tid == m_timerTest){
			if (m_connection){
				string msg("Time data");
				m_connection->Send(msg.data(), msg.length());
			}
		} else if(tid == m_timerReconn){
			cout << "start to reconnecting ... " << endl;
			Connect();
		}
	}
};


int main(int argc, char **argv)
{
// PropertyConfigurator::configure("log4cxx.cfg"); g_logger = Logger::getLogger("sdemo.cpp:main");
#ifdef WIN32
	WORD versionRequired;
	WSADATA wsaData;
	versionRequired = MAKEWORD(2, 2);
	int err = WSAStartup(versionRequired, &wsaData);
	if (err){
		cerr << "WSAStartup failed." << endl;
		return -1;
	}
#endif

	EventLooper &el = EventLooper::GetInstance();

	char mode = 's';
	string svrAddr = "127.0.0.1";
	short port = 5678;
	if (argc > 1)
		mode = argv[1][0];
	if (argc > 2)
		svrAddr = argv[2];
	if (argc > 3)
		port = atoi(argv[3]);

	cout << "run in mode " << mode << endl;

	if (mode == 's'){
		TestServer *server = new TestServer(svrAddr, 5678);
		if (!server->Start())
			cerr << "server start failed." << endl;
	} else {
		//string addr("10.1.49.253");
		TestClient *client = new TestClient(svrAddr, 5678);
		if (!client->Connect())
			cerr << "client connect " << svrAddr << " failed" << endl;
	}
	el.RunEventLoop();
	
#ifdef WIN32
	WSACleanup();
#endif

	return 0;
}
