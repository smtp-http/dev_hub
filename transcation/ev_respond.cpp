
#include "ev_respond.h"

using namespace std;

///////////////////////////////// ev_reciver ////////////////////////////

ev_reciver::ev_reciver()
	: m_client(NULL)
{
	return;
}

ev_reciver::~ev_reciver()
{

}

ev_reciver& ev_reciver::GetInstance()
{
	static ev_reciver _instance;
	return _instance;
}

void ev_reciver::UpdateEvent(std::string sectionName,std::string machineName,std::string eventName,char* eventData,unsigned int len)
{

}

//////////////////////////////////////// event ///////////////////////////////

void Event::SniffingPlcEvent()
{
	char rd_buf[EV_DATA_BUFF_LEN];
	string addr = m_flag + m_plcEventAddr;

	PlcProxy* plcProxy = &PlcProxy::Instance();
	
	if (m_machineContex == NULL) {
		printf("m_machineContex is NULL!  return.\n" );
		return;
	}

	unsigned int len = plcProxy->PlcReadWorlds(m_machineContex,addr,rd_buf);
	for(int i = 0;i < m_plcDataSize;i ++) {
		if(m_lastData[i] != rd_buf[i]) 
			m_evUpdater->UpdateEvent(m_sectionName,m_machineName,m_eventName,rd_buf,m_plcDataSize);
			break;
	}
}



//======================  Ev_register ======================



void Ev_Register::SendEapData(char *data)
{

}
