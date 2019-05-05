
#include "ev_respond.h"
#include "config.h"
#include <time.h>
using namespace std;


unsigned long GetTime()
{
	time_t now;
	time(&now);
	return (unsigned long)now;
}

///////////////////////////////// ev_reciver ////////////////////////////

ev_reciver::ev_reciver()
	: m_client(new TcpClient(SysConfig::Instance().GetUperComputerIp(),(short)SysConfig::Instance().GetUperComputerPort()))
{
	m_client->Connect();
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

void ev_reciver::UpdateEvent(std::string sectionName,std::string machineName,std::string eventName,unsigned char* eventData,unsigned int len)
{

}



//////////////////////////////////////// event ///////////////////////////////

/*
void Event::SniffingPlcEvent()
{
	unsigned char rd_buf[EV_DATA_BUFF_LEN];
	string addr = m_flag + m_plcEventAddr;

	PlcProxy* plcProxy = m_machineContex->GetProxy();//&PlcProxy::Instance();
	
	if (m_machineContex == NULL) {
		printf("m_machineContex is NULL!  return.\n" );
		return;
	}

	unsigned int len = plcProxy->PlcReadWorlds(addr,rd_buf,m_plcDataSize);
	for(int i = 0;i < m_plcDataSize;i ++) {
		if(m_lastData[i] != rd_buf[i]) {
			string sectionName = m_machineContex->SectionName();
			string machineName = m_machineContex->MachineName();
			m_evUpdater.UpdateEvent(sectionName,machineName,m_eventName,rd_buf,m_plcDataSize);
			break;
		}
	}
}


void Event::SendEapData(unsigned char* data)
{
	
}
*/

//======================  Ev_Register ======================

void Ev_Register::SendEapData(unsigned char* data)
{

}

void Ev_Register::SniffingPlcEvent()
{

}



//======================  Ev_EapCommand ======================


void Ev_EapCommand::SendEapData(unsigned char* data)
{

}

void Ev_EapCommand::SniffingPlcEvent()
{
	
}


//======================  Ev_HeareBeat ======================

void Ev_HeareBeat::SendEapData(unsigned char* data)
{
	PlcProxy* plcProxy = m_machineContex->GetProxy();
	string addr = m_flag + m_eapEventAddr;
	plcProxy->PlcWriteWorlds(addr,data,m_eapDataSize);
}


void Ev_HeareBeat::SniffingPlcEvent()
{
	unsigned char rd_buf[EV_DATA_BUFF_LEN];
	string addr = m_flag + m_plcEventAddr;

	PlcProxy* plcProxy = m_machineContex->GetProxy();//&PlcProxy::Instance();
	
	if (m_machineContex == NULL) {
		printf("m_machineContex is NULL!  return.\n" );
		return;
	}

	unsigned int len = plcProxy->PlcReadWorlds(addr,rd_buf,m_plcDataSize);
	struct Fream_HeartBeat_plc* hb_plc = (struct Fream_HeartBeat_plc*)rd_buf;
	if(m_sequenceID == hb_plc->SequenceID)
		cout << hb_plc->SequenceID << endl;
	#if 0
	for(int i = 0;i < m_plcDataSize;i ++) {

		if(m_lastData[i] != rd_buf[i]) {
			string sectionName = m_machineContex->SectionName();
			string machineName = m_machineContex->MachineName();

			m_evUpdater.UpdateEvent(sectionName,machineName,m_eventName,rd_buf,m_plcDataSize);
			break;
		}
	}
	#endif
}


/******************  Ev_EventChangedFlag  *********************/
void Ev_EventChangedFlag::SendEapData(unsigned char* data)
{

}


void Ev_EventChangedFlag::SniffingPlcEvent()
{
	
}


/******************  Ev_BreakPoint  *********************/
void Ev_BreakPoint::SendEapData(unsigned char* data)
{

}


void Ev_BreakPoint::SniffingPlcEvent()
{
	
}


/******************  Ev_NoticeMessage  *********************/
void Ev_NoticeMessage::SendEapData(unsigned char* data)
{

}


void Ev_NoticeMessage::SniffingPlcEvent()
{
	
}

/******************  Ev_AlarmCode  *********************/
void Ev_AlarmCode::SendEapData(unsigned char* data)
{

}


void Ev_AlarmCode::SniffingPlcEvent()
{
	
}

/******************  Ev_MachineStatus  *********************/
void Ev_MachineStatus::SendEapData(unsigned char* data)
{

}


void Ev_MachineStatus::SniffingPlcEvent()
{
	
}

/******************  Ev_MachineYield  *********************/
void Ev_MachineYield::SendEapData(unsigned char* data)
{

}


void Ev_MachineYield::SniffingPlcEvent()
{
	
}


/******************  Ev_CheckIn  *********************/

void Ev_CheckIn::SendEapData(unsigned char* data)
{

}


void Ev_CheckIn::SniffingPlcEvent()
{
	
}


/******************  Ev_PostResult  *********************/

void Ev_PostResult::SendEapData(unsigned char* data)
{

}


void Ev_PostResult::SniffingPlcEvent()
{
	
}


