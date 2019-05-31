
#include "ev_respond.h"
//#include "Http.h"
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
extern CHttpClient* GetHttpClient(string url);

ev_reciver::ev_reciver()
//	: m_client(new TcpClient(SysConfig::Instance().GetUperComputerIp(),(short)SysConfig::Instance().GetUperComputerPort()))
{
	if(SysConfig::Instance().UperProtocol == "tcp"){
		m_client = new TcpClient(SysConfig::Instance().GetUperComputerIp(),(short)SysConfig::Instance().GetUperComputerPort());
		//m_client->Connect();
	} else if (SysConfig::Instance().UperProtocol == "http") {
		m_client = GetHttpClient(SysConfig::Instance().UperUrl);
	} else {
		printf("%s:%d  UperProtocol is not define!\n",__FILE__,__LINE__);
		m_client = NULL;
	}
	
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
	string addr;  // fins: flag is a string, like "DM".  for modbus rtu ,it is null string

	cout << "********* : " << m_plcEventAddr << endl;

	PlcProxy* plcProxy = m_machineContex->GetProxy();//&PlcProxy::Instance();

	if ( plcProxy->GetProtocol() == PRO_FINS){
		addr = m_flag + m_plcEventAddr; 
	} else {
		addr = m_plcEventAddr;
	}
	
	if (m_machineContex == NULL) {
		printf("%s:%d  m_machineContex is NULL!\n",__FILE__,__LINE__);
		return -1;
	}

	if(plcProxy->GetConnectionStatus() == CONNECT_NO){
		printf("%s:%d  ConnectionStatus is CONNECT_NO!\n",__FILE__,__LINE__);
		return -2;
	}

	return plcProxy->PlcReadWorlds(addr,rd_buf,m_plcDataSize);
}


void Event::SendEapData(unsigned char* data)
{
	PlcProxy* plcProxy = m_machineContex->GetProxy();

	if(plcProxy->GetConnectionStatus() == CONNECT_NO){
		return;
	}


	string addr;                  // fins: flag is a string, like "DM".  for modbus rtu ,it is null string
	if ( plcProxy->GetProtocol() == PRO_FINS){
		addr = m_flag + m_eapEventAddr;
	} else {
		addr = m_plcEventAddr;
	}
	
	plcProxy->PlcWriteWorlds(addr,data,m_eapDataSize);
}


//======================  Ev_Register ======================

void Ev_Register::SendEapData(Json::Value &eap_data)
{

}

void Ev_Register::SniffingPlcEvent()
{

}



//======================  Ev_EapCommand ======================


void Ev_EapCommand::SendEapData(Json::Value &eap_data)
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


void Ev_EventChangedFlag::SniffingPlcEvent()
{
	
}


/******************  Ev_BreakPoint  *********************/


void Ev_BreakPoint::SniffingPlcEvent()
{
	
}


/******************  Ev_NoticeMessage  *********************/
void Ev_NoticeMessage::SendEapData(Json::Value &eap_data)
{

}


void Ev_NoticeMessage::SniffingPlcEvent()
{
	
}

/******************  Ev_AlarmCode  *********************/

void Ev_AlarmCode::SniffingPlcEvent()
{
	Json::Value root;
	Json::Value arraySections;
	Json::Value section;
	Json::Value arrayMachines;
	Json::Value machine;
	Json::Value arrayEvents;
	Json::Value event;
	Json::Value alarm_info;

	Json::Value alarmList;

	bool isHaveAlarm = false;

	unsigned char rd_buf[EV_DATA_BUFF_LEN];
	
	int ret = ReadData(rd_buf);
	if(ret != 0){
		printf("%s:%d  Ev_AlarmCode return error!\n",__FILE__,__LINE__);
		return;
	}
	

	struct Fream_AlarmCode_plc* ac_plc = (struct Fream_AlarmCode_plc*)rd_buf;
	if(m_sequenceID != BLEndianUshort(ac_plc->SequenceID,m_machineContex->WordSwap)){
		
		//printf("  alarm: ");
		//for(int i = 0; i < m_plcDataSize; i++) {
		//	printf("%x ",rd_buf[i]);
		//}

		//printf("\n");
		
		section["Name"] = m_machineContex->SectionName().c_str();
		machine["Name"] = m_machineContex->MachineName().c_str();
		event["Name"] = m_eventName;
		event["Action"]= m_eventAction;
		
		

		m_sequenceID = BLEndianUshort(ac_plc->SequenceID,m_machineContex->WordSwap);
		//cout << "---- s id: " << m_sequenceID << endl;
		//cout << "  Content: " << ac_plc->Content<<endl;
		//cout << "  ev_code: " << ac_plc->EventCode << endl;

		for(int i = 0;i < ALARM_CONTENT_LEN;i ++){
			cout << BLEndianUshort(ac_plc->Content[i],m_machineContex->WordSwap) << " ";
			for(int j = 0;j < 16;j ++) {
				if((BLEndianUshort(ac_plc->Content[i],m_machineContex->WordSwap) & (0x0001 << j)) != 0){
					cout << " *******$:  " << i << " " << j << endl;
					if(m_machineContex != NULL) {
						PlcAlarmInfo_t* alarm = m_machineContex->PlcAlarmCodeList[i*0x10 + j];
						if(alarm != NULL){
							if(!alarm->Enable){
								continue;
							}

							isHaveAlarm = true;

							alarm_info["Name"] = alarm->Name.c_str();
							alarm_info["Status"] = alarm->Status;
							alarm_info["Level"] = alarm->Level.c_str();

							alarmList.append(alarm_info);
							//cout << alarm->Name << " " << alarm->Enable << " " << alarm->WordOffset << " " << alarm->BitOffset;
						}
					}
				}
			}
		}

		if(isHaveAlarm){
			event["AlarmList"] = alarmList;
			arrayEvents.append(event);
			machine["EventList"] = arrayEvents;
			arrayMachines.append(machine);
			section["MachineList"] = arrayMachines;
			arraySections.append(section);
			root["SectionList"] = arraySections;
			
			string out = root.toStyledString();
			
			m_evUpdater.UpdateEvent(out.c_str(),out.length());
		}
		

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
	Json::Value root;
	Json::Value arraySections;
	Json::Value section;
	Json::Value arrayMachines;
	Json::Value machine;
	Json::Value arrayEvents;
	Json::Value event;
	//Json::Value status;

	unsigned char rd_buf[EV_DATA_BUFF_LEN];

	int ret = ReadData(rd_buf);
	if(ret != 0){
		printf("%s:%d  Ev_MachineStatus return error!\n",__FILE__,__LINE__);
		return;
	}



	struct Fream_MachineStatus_plc* ms_plc = (struct Fream_MachineStatus_plc*)rd_buf;
	if(m_sequenceID != BLEndianUshort(ms_plc->SequenceID,m_machineContex->WordSwap)){

		section["Name"] = m_machineContex->SectionName().c_str();
		machine["Name"] = m_machineContex->MachineName().c_str();
		event["Name"] = m_eventName;
		event["Action"]= m_eventAction;

		event["MachineStateCode"] = BLEndianUshort(ms_plc->MachineStateCode,m_machineContex->WordSwap);

		arrayEvents.append(event);
		machine["EventList"] = arrayEvents;
		arrayMachines.append(machine);
		section["MachineList"] = arrayMachines;
		arraySections.append(section);
		root["SectionList"] = arraySections;
		
		string out = root.toStyledString();

		m_evUpdater.UpdateEvent(out.c_str(),out.length());


		m_sequenceID = BLEndianUshort(ms_plc->SequenceID,m_machineContex->WordSwap);
		

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
	Json::Value root;
	Json::Value arraySections;
	Json::Value section;
	Json::Value arrayMachines;
	Json::Value machine;
	Json::Value arrayEvents;
	Json::Value event;

	unsigned char rd_buf[EV_DATA_BUFF_LEN];

	int ret = ReadData(rd_buf);
	if(ret != 0){
		printf("%s:%d  Ev_MachineStatus return error!\n",__FILE__,__LINE__);
		return;
	}


	struct Fream_MachineYield_plc* my_plc = (struct Fream_MachineYield_plc*)rd_buf;
	if(m_sequenceID != BLEndianUshort(my_plc->SequenceID,m_machineContex->WordSwap)){

		m_sequenceID = BLEndianUshort(my_plc->SequenceID,m_machineContex->WordSwap);

		section["Name"] = m_machineContex->SectionName().c_str();
		machine["Name"] = m_machineContex->MachineName().c_str();
		event["Name"] = m_eventName;
		event["Action"]= m_eventAction;

		event["Input"] = BLEndianIntSwap(my_plc->Input,m_machineContex->IntSwap);
		event["Output"] = BLEndianIntSwap(my_plc->Output,m_machineContex->IntSwap);
		event["Yield"] = BLEndianUshort(my_plc->Yield,m_machineContex->WordSwap);
		event["Oee"] = BLEndianUshort(my_plc->Oee,m_machineContex->WordSwap);

		string out = root.toStyledString();


		m_evUpdater.UpdateEvent(out.c_str(),out.length());

		struct Fream_MachineYield_eap my = {
			.SequenceID = m_sequenceID,
			.ResultCode = RESULT_Pass,
			.ResultValue = 0
		};

		SendEapData((unsigned char*)&my);
	}
}


/******************  Ev_CheckIn  *********************/



void Ev_CheckIn::SniffingPlcEvent()
{
	
}


/******************  Ev_PostResult  *********************/



void Ev_PostResult::SniffingPlcEvent()
{
	
}


