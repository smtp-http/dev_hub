
#include "ev_respond.h"
#include "config.h"
#include <time.h>
using namespace std;


unsigned short BLEndianUshort(unsigned short value,bool enable)
{
	if(enable)
    	return ((value & 0x00FF) << 8 ) | ((value & 0xFF00) >> 8);
    else 
    	return value;
}


unsigned int BLEndianUint32(unsigned int value) 
{ 
	return ((value & 0x000000FF) << 24) | ((value & 0x0000FF00) << 8) | ((value & 0x00FF0000) >> 8) | ((value & 0xFF000000) >> 24); 
}

unsigned int BLEndianIntSwap(unsigned int value,bool enable) 
{ 
	if(enable)
		return ((value & 0x000000FF) << 8) | ((value & 0x0000FF00) >> 8) | ((value & 0x00FF0000) << 8) | ((value & 0xFF000000) >> 8); 
	else
		return value;
}

unsigned long long BLEndianLongSwap(unsigned long long value,bool enable) 
{ 
	if(enable)
		return ((value & 0x00000000000000FF) << 8) | ((value & 0x000000000000FF00) >> 8) | ((value & 0x0000000000FF0000) << 8) | ((value & 0x00000000FF000000) >> 8) | \
				((value & 0x000000FF00000000) << 8) | ((value & 0x0000FF0000000000) >> 8) | ((value & 0x00FF000000000000) << 8) | ((value & 0xFF00000000000000) >> 8); 
	else
		return value;
}


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

void ev_reciver::UpdateEvent(const char* msg,unsigned int len)
{
	m_client->SendMsg(msg,len);
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

void Ev_AlarmCode::SniffingPlcEvent()
{
	unsigned char rd_buf[EV_DATA_BUFF_LEN];
	
	int ret = ReadData(rd_buf);
	if(ret != 0){
		printf("%s:%d  Ev_AlarmCode return error!\n",__FILE__,__LINE__);
		return;
	}
	

	struct Fream_AlarmCode_plc* ac_plc = (struct Fream_AlarmCode_plc*)rd_buf;
	if(m_sequenceID != BLEndianUshort(ac_plc->SequenceID,m_machineContex->WordSwap)){
		
		printf("  alarm: ");
		for(int i = 0; i < m_plcDataSize; i++) {
			printf("%x ",rd_buf[i]);
		}

		printf("\n");

		m_sequenceID = BLEndianUshort(ac_plc->SequenceID,m_machineContex->WordSwap);
		cout << "---- s id: " << m_sequenceID << endl;
		//cout << "  Content: " << ac_plc->Content<<endl;
		cout << "  ev_code: " << ac_plc->EventCode << endl;

		for(int i = 0;i < ALARM_CONTENT_LEN;i ++){
			cout << BLEndianUshort(ac_plc->Content[i],m_machineContex->WordSwap) << " ";
			for(int j = 0;j < 16;j ++) {
				if((BLEndianUshort(ac_plc->Content[i],m_machineContex->WordSwap) & (0x0001 << j)) != 0){
					cout << " *******$:  " << i << " " << j << endl;
					if(m_machineContex != NULL) {
						PlcAlarmInfo_t* alarm = m_machineContex->PlcAlarmCodeList[i*0x10 + j];
						if(alarm != NULL){
							cout << alarm->Name << " " << alarm->Enable << " " << alarm->WordOffset << " " << alarm->BitOffset;
						}
					}
				}
			}
		}
		cout << endl;

		m_evUpdater.UpdateEvent("alarm\n",7);

		struct Fream_AlarmCode_eap ac = {
			.SequenceID = m_sequenceID,
			.ResultCode = RESULT_Pass,
			.ResultValue = 0
		};

		SendEapData((unsigned char*)&ac);
	}
}

/******************  Ev_MachineStatus  *********************/


void Ev_MachineStatus::SniffingPlcEvent()
{
	unsigned char rd_buf[EV_DATA_BUFF_LEN];

	int ret = ReadData(rd_buf);
	if(ret != 0){
		printf("%s:%d  Ev_MachineStatus return error!\n",__FILE__,__LINE__);
		return;
	}


	struct Fream_MachineStatus_plc* ms_plc = (struct Fream_MachineStatus_plc*)rd_buf;
	if(m_sequenceID != BLEndianUshort(ms_plc->SequenceID,m_machineContex->WordSwap)){

		printf(" status: ");
		for(int i = 0; i < m_plcDataSize; i++) {
			printf("%x ",rd_buf[i]);
		}

		printf("\n");

		m_sequenceID = BLEndianUshort(ms_plc->SequenceID,m_machineContex->WordSwap);
		cout << "---- s id: " << m_sequenceID << endl;
		cout << "StateCode: " << ms_plc->MachineStateCode<<endl;
		cout << "  ev_code: " << ms_plc->EventCode << endl;

		m_evUpdater.UpdateEvent("status\n",8);

		struct Fream_MachineStatus_eap ms = {
			.SequenceID = m_sequenceID,
			.ResultCode = RESULT_Pass,
			.ResultValue = 0
		};

		SendEapData((unsigned char*)&ms);
	}
}

/******************  Ev_MachineYield  *********************/

void Ev_MachineYield::SniffingPlcEvent()
{
	unsigned char rd_buf[EV_DATA_BUFF_LEN];

	int ret = ReadData(rd_buf);
	if(ret != 0){
		printf("%s:%d  Ev_MachineStatus return error!\n",__FILE__,__LINE__);
		return;
	}


	struct Fream_MachineYield_plc* my_plc = (struct Fream_MachineYield_plc*)rd_buf;
	if(m_sequenceID != BLEndianUshort(my_plc->SequenceID,m_machineContex->WordSwap)){

		printf(" status: ");
		for(int i = 0; i < m_plcDataSize; i++) {
			printf("%x ",rd_buf[i]);
		}

		printf("\n");

		m_sequenceID = BLEndianUshort(my_plc->SequenceID,m_machineContex->WordSwap);
		cout << "---- s id: " << m_sequenceID << endl;
		cout << "  ev_code: " << BLEndianUshort(my_plc->EventCode,m_machineContex->WordSwap) << endl;
		cout << "    Input: " << BLEndianIntSwap(my_plc->Input,m_machineContex->IntSwap) << endl;
		cout << "   Output: " << BLEndianIntSwap(my_plc->Output,m_machineContex->IntSwap) << endl;
		cout << "    Yield: " << BLEndianUshort(my_plc->Yield,m_machineContex->WordSwap) << endl;
		cout << "      Oee: " << BLEndianUshort(my_plc->Oee,m_machineContex->WordSwap) << endl;

		m_evUpdater.UpdateEvent("yield\n",8);

		struct Fream_MachineYield_eap my = {
			.SequenceID = m_sequenceID,
			.ResultCode = RESULT_Pass,
			.ResultValue = 0
		};

		SendEapData((unsigned char*)&my);
	}
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


