#include <stdio.h>
#include <string>
#include <string.h>
#include <list>
#include <iostream>
#include <algorithm>
#include <json/json.h>
#include "connbase.h"
#include "eventlooper.h"
#include "CommServer.h"


using namespace lux;
using namespace std;

CommServer::CommServer()
{
	cout << "commserver +++++" << endl;
	m_transManager = NULL;
}

void CommServer::SetTransManager(ITranscationOwner * tm)
{
	m_transManager = tm;
}



void CommServer::OnConnection(Connection *conn, IConnectorAcceptor *ca)
{
	Session *session;
		
	if (!ca){
		printf("%s:%d    acceptor is null!",__FILE__,__LINE__);
		return;
	}

	if (!conn) {
		printf("%s:%d    connection is null!",__FILE__,__LINE__);
		return;
	}
	

	if (ca->IsConnector() == false) {
		if (ca == m_acceptor) {
			session = new Session(this);
			if (session == NULL) {
				printf("%s:%d    new session is null!",__FILE__,__LINE__);
			}

			conn->SetConnectionSink(session);
			session->SetConnection(conn);
			m_connections.push_back(conn);

			///////////////// sub transcation /////////

			string peerIp; 
			list<int> transIds;
			list<int>::iterator iter;

			if(conn->GetPeerAddr(peerIp)) {
				cout << "peerIp: " << peerIp << endl;
				
			}

			m_transManager->GetTransIds(transIds,peerIp);

			if ( !transIds.empty()) {
				for ( iter = transIds.begin(); iter != transIds.end(); iter ++ ) {
					cout << "---- trans id(*iter): " << *iter << endl;
					SubscibeTranscationId(*iter,session);
					session->M_Task()->AddTranscation(*iter);
					m_transManager->SetTransTask(*iter,session->M_Task());
				}
			}

			OnNewSession(*session);
		}
	}
}

void CommServer::UpdateInfo(const char *buf,int len)
{
	list<Connection *>::iterator it;
	char send[1025];
	strncpy(send,buf,len);
	send[len] = '\0';
	for (it = m_connections.begin(); it != m_connections.end(); ++it){
		Connection *con = (Connection *)(*it);
		//con->Send(data,len);
		con->Send(send,len + 1);
	}
}

void CommServer::Update(const string data,unsigned int transId)
{
	SessionList::iterator iter;
	Session *s;

	if(m_transcationSessions.find(transId) == m_transcationSessions.end()) {
		printf("%s:%d    This TranscationSessions is not exist!",__FILE__,__LINE__);
		return;
	}
	
	for (iter = m_transcationSessions[transId]->begin();iter != m_transcationSessions[transId]->end();iter++) { 
		s = *iter;
		s->SendMessage(data);
	}
}



bool CommServer::SubscibeTranscationId(int id,Session *s)
{
	SessionList::iterator iter;

	cout << "Sub trans, id: " << id << "session: " << s << endl;
	
	if(m_transcationSessions.find(id) == m_transcationSessions.end()) {
		SessionList *list = new SessionList;
		//m_transcationSessions.insert(pair<int,*SessionList>(id,list));
		cout << "push back -- " << endl;
		list->push_back(s);
		m_transcationSessions[id] = list;

		return true;
	} else {
		iter = std::find(m_transcationSessions[id]->begin(),m_transcationSessions[id]->end(),s);
		if(iter != m_transcationSessions[id]->end()){
			printf("%s:%d    This session is already exist!",__FILE__,__LINE__);
			return false;
		}
		
		m_transcationSessions[id]->push_back(s);
		return true;
	}

	return false;
}



bool CommServer::RemoveSessionInMap(Session *session)// 改为session
{
	SessionList::iterator iter;
	Session *s;
	TranscationSessions::iterator tc;
	SessionList *sl;
	

	for(tc = m_transcationSessions.begin();tc != m_transcationSessions.end();){

		sl = (SessionList *)tc->second;
		
		for (iter = sl->begin();iter != sl->end();) { 
			s = *iter;
			if(s == session){
				sl->erase(iter ++);	 
				break;
			}

			iter ++;
		}

		if (sl->empty()) {
			delete sl;
			m_transcationSessions.erase(tc++);
		} else {
			tc ++;
		}

	}

	return true;
}


void CommServer::SetDownChannel(IDownChannel *dc)
{
	if (dc != NULL) {
		m_downChannel = dc;
	}
}

bool CommServer::OnFrame(string &frame,Session *session)
{
	cout << "OnFrame" << endl;

	int transId;
	SessionList::iterator iter;
	TranscationSessions::iterator tc;
	SessionList *sl;

	cout << "frame: " << frame << endl;

	

	if (session->M_Task()->GetTaskStatus() != task_idle) {

		Json::Value root;

		root["cmd"] = "Busy";
		//root["ReqId"] = value["ReqId"].asInt();

		string res = root.toStyledString();
		
		session->SendMessage(res);

		return true;
	}


	for(tc = m_transcationSessions.begin();tc != m_transcationSessions.end();tc ++){
		transId = (int)tc->first; 
		cout << "transcation id: " << transId << endl;
		sl = (SessionList *)tc->second;
		iter = std::find(sl->begin(),sl->end(), session);
		if(iter != sl->end()) {
			cout << "trans id: " << transId << endl;
			//session->M_Task()->SetStatus(task_accept);
			m_downChannel->OnDownStream(transId,frame);
		}
	}

	return true;
}


void CommServer::OnNewSession(lux :: Session &session)
{
	cout << "OnNewSession" << endl;
}

void CommServer::OnSessionClose(Session &session)
{
	cout << "OnSessionClose" << endl;
	
	list<Session*>::iterator alit;
	Session *se;

		
	for (alit = m_sessionList.begin();alit != m_sessionList.end();alit ++) {
		se = *alit;
		if(se == &session){
			m_sessionList.erase(alit);  
				
			break;
		}
	}	
}


