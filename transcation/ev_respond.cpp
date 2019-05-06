
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


int Event::ReadData(unsigned char *rd_buf)
{
	string addr = m_flag + m_plcEventAddr;

	PlcProxy* plcProxy = m_machineContex->GetProxy();//&PlcProxy::Instance();
	
	if (m_machineContex == NULL) {
		printf("m_machineContex is NULL!  return.\n" );
		return -1;
	}

	if(plcProxy->GetConnectionStatus() == CONNECT_NO)
		return -2;

	return plcProxy->PlcReadWorlds(addr,rd_buf,m_plcDataSize);
}


void Event::SendEapData(unsigned char* data)
{
	PlcProxy* plcProxy = m_machineContex->GetProxy();

	if(plcProxy->GetConnectionStatus() == CONNECT_NO){
		return;
	}


	string addr = m_flag + m_eapEventAddr;
	plcProxy->PlcWriteWorlds(addr,data,m_eapDataSize);
}


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


void Ev_HeareBeat::SniffingPlcEvent()
{
	unsigned char rd_buf[EV_DATA_BUFF_LEN];
	
	int ret = ReadData(rd_buf);
	
	if(ret != 0){
		printf("%s:%d  Ev_HeareBeat return error!\n",__FILE__,__LINE__);
		return;
	}

	struct Fream_HeartBeat_plc* hb_plc = (struct Fream_HeartBeat_plc*)rd_buf;
	/*if(m_sequenceID == hb_plc->SequenceID){
		cout << "--- s id:"<< hb_plc->SequenceID << endl;
		cout << "    time:" << hb_plc->TimeTicks<<endl;
		cout << " ev_code:" << hb_plc->EventCode << endl;
	}*/
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
	unsigned char rd_buf[EV_DATA_BUFF_LEN];
	
	int ret = ReadData(rd_buf);
	if(ret != 0){
		printf("%s:%d  Ev_AlarmCode return error!\n",__FILE__,__LINE__);
		return;
	}
	printf("  alarm: ");
	for(int i = 0; i < m_plcDataSize; i++) {
		printf("%x ",rd_buf[i]);
	}

	printf("\n");

	struct Fream_AlarmCode_plc* ac_plc = (struct Fream_AlarmCode_plc*)rd_buf;
	/*if(m_sequenceID == ac_plc->SequenceID){
		m_sequenceID = ac_plc->SequenceID;
		cout << "---- s id: " << ac_plc->SequenceID << endl;
		cout << "  Content: " << ac_plc->Content<<endl;
		cout << "  ev_code: " << ac_plc->EventCode << endl;
	}*/
}

/******************  Ev_MachineStatus  *********************/
void Ev_MachineStatus::SendEapData(unsigned char* data)
{

}


void Ev_MachineStatus::SniffingPlcEvent()
{
	unsigned char rd_buf[EV_DATA_BUFF_LEN];

	int ret = ReadData(rd_buf);
	if(ret != 0){
		printf("%s:%d  Ev_MachineStatus return error!\n",__FILE__,__LINE__);
		return;
	}

	printf(" status: ");
	for(int i = 0; i < m_plcDataSize; i++) {
		printf("%x ",rd_buf[i]);
	}

	printf("\n");

	struct Fream_MachineStatus_plc* ms_plc = (struct Fream_MachineStatus_plc*)rd_buf;
	/*if(m_sequenceID == ms_plc->SequenceID){
		m_sequenceID = ms_plc->SequenceID;
		cout << "---- s id: " << ms_plc->SequenceID << endl;
		cout << "StateCode: " << ms_plc->MachineStateCode<<endl;
		cout << "  ev_code: " << ms_plc->EventCode << endl;
	}*/
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


