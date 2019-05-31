#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include <string>
#include <vector>
#include <iostream>
//#include <algorithm>
#include "connbase.h"
#include "eventlooper.h"
#include "Client.h"

using namespace std;
using namespace lux;




class TcpClient : public IConnectorAcceptorSink, public IConnectionSink, public ITimerUserSink, public Client
{
	string m_peerAddr;
	short m_peerPort;
	Connector *m_connector;
	Connection *m_connection;
	TimerID m_timerReconn;
	TimerID m_timerTest;

	//IFrameRecver* m_frameRecver;
	vector<char> m_rdBuffer;
public:
	TcpClient(const string &peerAddr, short peerPort)
		: m_peerAddr(peerAddr)
		, m_peerPort(peerPort)
		, m_connector(NULL)
		, m_connection(NULL)
		, m_timerReconn(-1)
		, m_timerTest(-1)
	{
		Connect();
	}

	

	virtual int SendMsg(const char* buf,unsigned int len)
	{
		return m_connection->Send(buf, len);
	}

	
protected:
	bool Connect()
	{
		if (m_connector)
			return true;
		m_connector = new Connector(m_peerAddr, m_peerPort, this);
		return m_connector->Connect(3000) == 0; //3 seconds
	}

	virtual void OnConnection(Connection *conn, IConnectorAcceptor *ca){
		if (conn){
			//cout << "Client: connected succeed to " << conn->GetPeerAddress() << ":" << conn->GetPeerPort() << endl;
			m_connection = conn;
			conn->SetConnectionSink(this);

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
		int i,len = length,count = 0;
		string newFrame;
		int pos=0;
		len =0;

		
		if(buf == NULL){
			printf("buf == NULL , [%s][%s][%d]\n",__FILE__,__PRETTY_FUNCTION__,__LINE__);
		}

	    
		for(i=0;i<length;i++){
			if(buf[i] == '\0'){
				len = i-pos;
				pos = i;
				newFrame.clear();
				newFrame.assign(m_rdBuffer.begin(),m_rdBuffer.end());
				len += m_rdBuffer.size();

				if(len>1){
					if(m_frameRecver != NULL)
						m_frameRecver->OnFrame(newFrame);
					m_rdBuffer.clear();
				}
			} else {
				m_rdBuffer.push_back(buf[i]);
			}        
		}
	}

	virtual void OnWrite(Connection *conn){
		cout << "OnWrite" << endl;
	}

	virtual void OnDisconnect(int reason, Connection *conn)
	{
		//cout << "OnDisconnect from " << conn->GetPeerAddress() << ":" << conn->GetPeerPort() << endl;
		delete m_connection;
		m_connection = NULL;

		delete m_connector;
		m_connector = NULL;

		if (m_timerReconn == -1){
			struct timeval tv={1, 0};
			m_timerReconn = EventLooper::GetInstance().ScheduleTimer(&tv, TF_FIRE_PERIODICALLY, this);
		}
		//EventLooper &el = EventLooper::GetInstance();

		//el.StopEventLoop(100);
	}

	virtual void OnTimer(TimerID tid)
	{
		//cout << "OnTimer " << tid << endl;
		if(tid == m_timerReconn){
			cout << "start to reconnecting ... " << endl;
			EventLooper::GetInstance().CancelTimer(m_timerReconn);
			m_timerReconn = -1;
			Connect();
		}
	}
};



#endif
