
#include "TcpClient.h"


bool TcpClient::Connect()
{
	if (m_connector)
		return true;

	m_connector = new Connector(m_peerAddr, m_peerPort, this);
	return m_connector->Connect(3000) == 0; //3 seconds
}

void TcpClient::OnConnection(Connection *conn, IConnectorAcceptor *ca)
{
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


void TcpClient::OnData(const char *buf, int length, Connection *conn){
	cout << "OnData" << endl;
	string s(buf, length);
	cout << "recv: " << s << endl;
	//conn->Send(buf, length);
	//EventLooper::GetInstance().StopEventLoop(3000);
}

void TcpClient::OnWrite(Connection *conn){
	cout << "OnWrite" << endl;
}

void TcpClient::OnDisconnect(int reason, Connection *conn)
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

void TcpClient::OnTimer(TimerID tid)
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