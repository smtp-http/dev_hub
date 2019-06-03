#ifndef __EV_RESPOND_H__
#define __EV_RESPOND_H__

#include <string>
#include <string.h>
#include "config.h"
#include "TcpClient.h"
#include "Http.h"
#include "PlcProxy.h"





//////////////////////////////////////////// RESULT CODE ///////////////////////////////////////////////
#define RESULT_Unkown       0x00	//未知
#define RESULT_Pass			0x10 	//成功
#define RESULT_Fail			0x02	//失败
#define RESULT_Dump			0x03	//报废丢弃
#define RESULT_EmptyCarry	0x04	//空载具
#define RESULT_Ignore		0x05	//直接过站
#define RESULT_Retest		0x06	//重测

#define RESULT_NoBarCode	0x10	//没有码
#define RESULT_InvalidBarCode	0x11	//无效的码
#define RESULT_InvalidCarrierSN	0x12	//无效的载具码
#define RESULT_InvalidTraySN	0x13	//无效的Tray盘码
#define RESULT_IllegalCarrier	0x14	//非法载具
#define RESULT_InvalidTray	0x15	//非法Tray盘
#define RESULT_BindFailed	0x16	//绑定失败
#define RESULT_PrintError	0x17	//打印错误
#define RESULT_HandShakeFailed	0x18	//握手失败
#define RESULT_PreDefineEventFormatError	0x19	//配置区定义错误
#define RESULT_CarrierOccupy	0x1A	//载具冲突，未解绑
#define RESULT_RepeatedProcess	0x1B	//重复加工
#define RESULT_PrinterLost	0x1C	//打印机未配置
#define RESULT_PrinterOffline	0x1D	//打印机离线
#define RESULT_PrinterDisabled	0x1E	//打印机被禁用

#define RESULT_MesCheckInFailed	0x30	//Mes返回禁止加工
#define RESULT_MesRespTimeout	0x31	//Mes返回超时
#define RESULT_MesServerOffline	0x32	//Mes服务器掉线
#define RESULT_EapRespTimeout	0x33	//Eap返回超时
#define RESULT_EapServerOffline	0x34	//Eap服务器掉线
#define RESULT_EapClientOffline	0x35	//Eap客户端掉线
#define RESULT_PlcRespTimeout	0x36	//Plc返回超时
#define RESULT_PlcRespInvalidSID	0x37	//Plc返回的SID错误

#define RESULT_StationDisabled	0x50	//工位被屏蔽
#define RESULT_StationNotExisting	0x51	//工位未定义
#define RESULT_EapClientException	0x52	//Eap程序发生异常


//////////////////////////////////////////// Event Property ///////////////////////////////////////////
#define Ev_Pro_None	0	
#define Ev_Pro_Mirror	0x01	//镜像模式



////////////////////////////////////////////  DATA structrue ///////////////////////////////////////////

/******************  HeartBeat  *********************/
struct Fream_HeartBeat_plc {
	unsigned short SequenceID;
	unsigned short EventCode;
	unsigned long TimeTicks;
}__attribute__((__packed__));

struct Fream_HeartBeat_eap {
	unsigned short SequenceID;
	unsigned short ResultCode;
	unsigned long TimeTicks;
}__attribute__((__packed__));


/******************  Register  *********************/
struct Fream_Register_plc {
	unsigned short SequenceID;
	unsigned short EventCode;
	unsigned char EapIp;
	unsigned char PlcIp;
}__attribute__((__packed__));

struct Fream_Register_eap {
	unsigned short SequenceID;
	unsigned short ResultCode;
	unsigned char EapIp;
	unsigned char PlcIp;
}__attribute__((__packed__));

/******************  EapCommand  *********************/
struct Fream_EapCommand_plc {
	unsigned short SequenceID;
	unsigned short EventCode;
	unsigned short CommandCode;
}__attribute__((__packed__));

struct Fream_EapCommand_eap {
	unsigned short SequenceID;
	unsigned short ResultCode;
	unsigned short CommandCode;
}__attribute__((__packed__));

/******************  NoticeMessage  *********************/
struct Fream_NoticeMessage_plc {
	unsigned short SequenceID;
	unsigned short EventCode;
	unsigned char MessageText[60];
}__attribute__((__packed__));

struct Fream_NoticeMessage_eap {
	unsigned short SequenceID;
	unsigned short ResultCode;
	unsigned char MessageText[60];
}__attribute__((__packed__));


/******************  EventChangedFlag  *********************/
struct Fream_EventChangedFlag_plc {
	unsigned short SequenceID;
	unsigned short EventCode;
	unsigned long EventFlags;
}__attribute__((__packed__));

struct Fream_EventChangedFlag_eap {
	unsigned short SequenceID;
	unsigned short ResultCode;
	unsigned long EventFlags;
}__attribute__((__packed__));


/******************  BreakPoint  *********************/
struct Fream_BreakPoint_plc {
	unsigned short SequenceID;
	unsigned short EventCode;
	unsigned char Content[40];
}__attribute__((__packed__));

struct Fream_BreakPoint_eap {
	unsigned short SequenceID;
	unsigned short ResultCode;
	unsigned short ResultValue;
}__attribute__((__packed__));


/******************  AlarmCode  *********************/
#define ALARM_CONTENT_LEN 20
struct Fream_AlarmCode_plc {
	unsigned short SequenceID;
	unsigned short EventCode;
	unsigned short Content[ALARM_CONTENT_LEN];
}__attribute__((__packed__));

struct Fream_AlarmCode_eap {
	unsigned short SequenceID;
	unsigned short ResultCode;
	unsigned short ResultValue;
}__attribute__((__packed__));


/******************  MachineStatus  *********************/
struct Fream_MachineStatus_plc {
	unsigned short SequenceID;
	unsigned short EventCode;
	unsigned short MachineStateCode;
}__attribute__((__packed__));

struct Fream_MachineStatus_eap {
	unsigned short SequenceID;
	unsigned short ResultCode;
	unsigned short ResultValue;
}__attribute__((__packed__));


/******************  MachineYield  *********************/
struct Fream_MachineYield_plc {
	unsigned short SequenceID;
	unsigned short EventCode;
	unsigned int  Input;
	unsigned int  Output;
	unsigned short Yield;
	unsigned short Oee;
}__attribute__((__packed__));

struct Fream_MachineYield_eap {
	unsigned short SequenceID;
	unsigned short ResultCode;
	unsigned short ResultValue;
}__attribute__((__packed__));


/******************  CheckIn  *********************/
struct Fream_CheckIn_plc {
	unsigned short SequenceID;
	unsigned short EventCode;
	unsigned char SerialNumber[30];
}__attribute__((__packed__));

struct Fream_CheckIn_eap {
	unsigned short SequenceID;
	unsigned short ResultCode;
	unsigned int ResultValue;
}__attribute__((__packed__));


/******************  PostResult  *********************/
struct Fream_PostResult_plc {
	unsigned short SequenceID;
	unsigned short EventCode;
	unsigned int ResultCode;
	unsigned char SerialNumber[30];
}__attribute__((__packed__));

struct Fream_PostResult_eap {
	unsigned short SequenceID;
	unsigned short ResultCode;
	unsigned int ResultValue;
}__attribute__((__packed__));

////////////////////////////////////////////  EVENT ////////////////////////////////////////////////////


#define EV_PARAM_INIT(ev_para) m_eventAction = ev_para->eventAction; \
		m_eventName = ev_para->eventName; \
		m_plcEventAddr = ev_para->plcEventAddr; \
		m_plcDataSize = ev_para->plcDataSize; \
		m_eapEventAddr = ev_para->eapEventAddr; \
		m_eapDataSize = ev_para->eapDataSize; \
		m_flag = ev_para->flag;

struct EvPara
{
	std::string  eventName;
	std::string  eventAction;
	std::string  plcEventAddr;
	unsigned int plcDataSize;
	std::string  eapEventAddr;
	unsigned int eapDataSize;
	std::string  flag;         // fins: flag is a string, like "DM".  for modbus rtu ,it is null string
};

class IEventUpdater
{
public:
	virtual void UpdateEvent(const char* msg,unsigned int len) = 0;//(std::string sectionName,std::string machineName,std::string eventName,unsigned char* data,unsigned int len) = 0;
};

class ev_reciver : public IEventUpdater
{
public:
	virtual ~ev_reciver();

	static ev_reciver& GetInstance();
	virtual void UpdateEvent(const char* msg,unsigned int len);//(std::string sectionName,std::string machineName,std::string eventName,unsigned char* data,unsigned int len);

	friend class Director;
private:
	ev_reciver();

	Client* m_client;
};

#define EV_DATA_BUFF_LEN 1000

class Event {
public:
	Event()
		: m_evUpdater(ev_reciver::GetInstance())
		, m_machineContex(NULL)
		, m_sequenceID(-1)
	{

	}

	Event(struct EvPara* ev_para)
		: m_evUpdater(ev_reciver::GetInstance())
	{
		EV_PARAM_INIT(ev_para)
	}

	virtual ~Event(){}

	virtual void SniffingPlcEvent() = 0;
	virtual void SendEapData(unsigned char* data);
	virtual void SendEapData(Json::Value &eap_data)
	{}

	void SetMachineContex(MachineContex* mc)
	{
		m_machineContex = mc;
	}

	unsigned short GetNextSequenceId()
	{
		m_sequenceID += 1; 
		return m_sequenceID;
	}

protected:

	int ReadData(unsigned char*);

	unsigned int m_direction;  // 0 ---- EAP-->PLC         1 ---- PLC-->EAP

	std::string m_eventName;
	std::string m_eventAction;

	std::string m_flag;     // "DM"

	std::string m_plcEventAddr;
	unsigned int m_plcDataSize;    // word

	std::string m_eapEventAddr;      // 
	unsigned int m_eapDataSize;

	IEventUpdater& m_evUpdater;

	MachineContex *m_machineContex;

//	char m_lastData[EV_DATA_BUFF_LEN];

	unsigned short m_sequenceID;
};


class Ev_Register : public Event
{
public:
	Ev_Register(struct EvPara* ev_para)
	{
		m_direction = 0;   // EAP-->PLC
		EV_PARAM_INIT(ev_para)
	}

	virtual ~Ev_Register(){}
	virtual void SendEapData(Json::Value &eap_data);
	virtual void SniffingPlcEvent();
};


class Ev_EapCommand : public Event
{
public:
	Ev_EapCommand(struct EvPara* ev_para)
	{
		m_direction = 0;
		EV_PARAM_INIT(ev_para)
	}
	virtual ~Ev_EapCommand(){}
	virtual void SendEapData(Json::Value &eap_data);
	virtual void SniffingPlcEvent();
};


///////////////////// heare beat /////////////////////////


class Ev_HeareBeat : public Event
{
public:
	Ev_HeareBeat(struct EvPara* ev_para)
	{
		m_direction = 0; // EAP-->PLC
		EV_PARAM_INIT(ev_para)
	}
	virtual ~Ev_HeareBeat(){}

	virtual void SniffingPlcEvent();
};


/******************  Ev_EventChangedFlag  *********************/

class Ev_EventChangedFlag : public Event
{
public:
	Ev_EventChangedFlag(struct EvPara* ev_para)
	{
		m_direction = 1; // PLC-->EAP
		EV_PARAM_INIT(ev_para)
	}
	virtual ~Ev_EventChangedFlag(){}
	//virtual void SendEapData(unsigned char* data);
	virtual void SniffingPlcEvent();
};




/******************  Ev_NoticeMessage  *********************/

class Ev_NoticeMessage : public Event
{
public:
	Ev_NoticeMessage(struct EvPara* ev_para)
	{
		m_direction = 0; // EAP-->PLC
		EV_PARAM_INIT(ev_para)
	}
	virtual ~Ev_NoticeMessage(){}
	virtual void SendEapData(Json::Value &eap_data);
	virtual void SniffingPlcEvent();
};


/******************  Ev_BreakPoint  *********************/

class Ev_BreakPoint : public Event
{
public:
	Ev_BreakPoint(struct EvPara* ev_para)
	{
		m_direction = 1; // PLC-->EAP
		EV_PARAM_INIT(ev_para)
	}
	virtual ~Ev_BreakPoint(){}
	//virtual void SendEapData(unsigned char* data);
	virtual void SniffingPlcEvent();
};

/******************  Ev_AlarmCode  *********************/

class Ev_AlarmCode : public Event
{
public:
	Ev_AlarmCode(struct EvPara* ev_para)
	{
		m_direction = 1; // PLC-->EAP
		EV_PARAM_INIT(ev_para)
	}
	virtual ~Ev_AlarmCode(){}
	
	virtual void SniffingPlcEvent();
};

/******************  Ev_MachineStatus  *********************/
class Ev_MachineStatus : public Event
{
public:
	Ev_MachineStatus(struct EvPara* ev_para)
	{
		m_direction = 1; // PLC-->EAP
		EV_PARAM_INIT(ev_para)
	}
	virtual ~Ev_MachineStatus(){}
	
	virtual void SniffingPlcEvent();
};

/******************  Ev_MachineYield  *********************/
class Ev_MachineYield : public Event
{
public:
	Ev_MachineYield(struct EvPara* ev_para)
	{
		m_direction = 1; // PLC-->EAP
		EV_PARAM_INIT(ev_para)
	}
	virtual ~Ev_MachineYield(){}
	
	virtual void SniffingPlcEvent();
};


/******************  Ev_CheckIn  *********************/
class Ev_CheckIn : public Event
{
public:
	Ev_CheckIn(struct EvPara* ev_para)
	{
		m_direction = 1; // PLC-->EAP
		EV_PARAM_INIT(ev_para)
	}
	virtual ~Ev_CheckIn(){}
	//virtual void SendEapData(unsigned char* data);
	virtual void SniffingPlcEvent();
};


/******************  Ev_PostResult  *********************/
class Ev_PostResult : public Event
{
public:
	Ev_PostResult(struct EvPara* ev_para)
	{
		m_direction = 1; // PLC-->EAP
		EV_PARAM_INIT(ev_para)
	}
	virtual ~Ev_PostResult(){}
	//virtual void SendEapData(unsigned char* data);
	virtual void SniffingPlcEvent();
};

unsigned long GetTime();

#endif
