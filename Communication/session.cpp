#include <stdio.h>
#include "session.h"
#include "CommServer.h"


using namespace lux;
using namespace std;

ITaskUse *BuildTask(IMsgSender *msg);

Session::Session(CommServer *server)
	: m_server(server)
{
	m_task = BuildTask(this);
}

Session::~Session()
{
	if(m_task != NULL){
		delete m_task;
		m_task = NULL;
	}
}



void Session::OnData(const char *buf, int length, Connection *conn)
{
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
				//OnFrame(newFrame);
				m_server->OnFrame(newFrame,this);
				m_rdBuffer.clear();
			}
		} else {
			m_rdBuffer.push_back(buf[i]);
		}		 
	}
	
	return;

}

int Session::SendMessage(string str)
{
	int ret;

	str =str+'\0';
	if(m_connection != NULL){
		ret = m_connection->Send(str.data(),str.length());

		if(ret < 0){
			printf("%s:Session::%s:%d",__FILE__,__PRETTY_FUNCTION__,__LINE__);
		}
		
		return ret;
	}

	return -1;
}


void Session::Close()
{
	if (m_connection){
		m_connection->Close();
		delete m_connection;
		m_connection = NULL;
	}
	
	delete this;
}

void Session::SetConnection(Connection *connection)
{
	m_connection = connection;
}

void Session::OnWrite(Connection *conn)
{

}


void Session::OnDisconnect(int reason, Connection *conn)
{
	m_server->RemoveSessionInMap(this);
	m_server->OnSessionClose(*this);
	Close();
}








