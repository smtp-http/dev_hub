#ifndef __EV_RESPOND_H__
#define __EV_RESPOND_H__

#include <string>
#include <string.h>
#include "TcpClient.h"
#include "PlcProxy.h"


////////////////////////////////////////////  DATA structrue ///////////////////////////////////////////

/******************  HeartBeat  *********************/
struct Fream_HeartBeat_eap {
	unsigned short SequenceID;
	unsigned short EventCode;
	unsigned long long TimeTicks;
}__attribute__((__packed__));

struct Fream_HeartBeat_plc {
	unsigned short SequenceID;
	unsigned short ResultCode;
	unsigned long long TimeTicks;
}__attribute__((__packed__));


/******************  Register  *********************/
struct Fream_Register_eap {
	unsigned short SequenceID;
	unsigned short EventCode;
	unsigned char EapIp;
	unsigned char PlcIp;
}__attribute__((__packed__));

struct Fream_Register_plc {
	unsigned short SequenceID;
	unsigned short ResultCode;
	unsigned char EapIp;
	unsigned char PlcIp;
}__attribute__((__packed__));

/******************  EapCommand  *********************/
struct Fream_EapCommand_eap {
	unsigned short SequenceID;
	unsigned short EventCode;
	unsigned short CommandCode;
}__attribute__((__packed__));

struct Fream_EapCommand_plc {
	unsigned short SequenceID;
	unsigned short ResultCode;
	unsigned short CommandCode;
}__attribute__((__packed__));

/******************  NoticeMessage  *********************/
struct Fream_NoticeMessage_eap {
	unsigned short SequenceID;
	unsigned short EventCode;
	unsigned char MessageText[60];
}__attribute__((__packed__));

struct Fream_NoticeMessage_plc {
	unsigned short SequenceID;
	unsigned short ResultCode;
	unsigned char MessageText[60];
}__attribute__((__packed__));


/******************  EventChangedFlag  *********************/
struct Fream_EventChangedFlag_eap {
	unsigned short SequenceID;
	unsigned short EventCode;
	unsigned long long EventFlags;
}__attribute__((__packed__));

struct Fream_EventChangedFlag_plc {
	unsigned short SequenceID;
	unsigned short ResultCode;
	unsigned long long EventFlags;
}__attribute__((__packed__));


/******************  BreakPoint  *********************/
struct Fream_BreakPoint_eap {
	unsigned short SequenceID;
	unsigned short EventCode;
	unsigned char Content[40];
}__attribute__((__packed__));

struct Fream_BreakPoint_plc {
	unsigned short SequenceID;
	unsigned short ResultCode;
	unsigned short ResultValue;
}__attribute__((__packed__));


/******************  AlarmCode  *********************/
struct Fream_AlarmCode_eap {
	unsigned short SequenceID;
	unsigned short EventCode;
	unsigned char Content[40];
}__attribute__((__packed__));

struct Fream_AlarmCode_plc {
	unsigned short SequenceID;
	unsigned short ResultCode;
	unsigned short ResultValue;
}__attribute__((__packed__));


/******************  MachineStatus  *********************/
struct Fream_MachineStatus_eap {
	unsigned short SequenceID;
	unsigned short EventCode;
	unsigned short MachineStateCode;
}__attribute__((__packed__));

struct Fream_MachineStatus_plc {
	unsigned short SequenceID;
	unsigned short ResultCode;
	unsigned short ResultValue;
}__attribute__((__packed__));


/******************  MachineYield  *********************/
struct Fream_MachineYield_eap {
	unsigned short SequenceID;
	unsigned short EventCode;
	unsigned int  Input;
	unsigned int  Output;
	unsigned short Yield;
	unsigned short Oee;
}__attribute__((__packed__));

struct Fream_MachineYield_plc {
	unsigned short SequenceID;
	unsigned short ResultCode;
	unsigned short ResultValue;
}__attribute__((__packed__));


/******************  CheckIn  *********************/
struct Fream_CheckIn_eap {
	unsigned short SequenceID;
	unsigned short EventCode;
	unsigned char SerialNumber[30];
}__attribute__((__packed__));

struct Fream_CheckIn_plc {
	unsigned short SequenceID;
	unsigned short ResultCode;
	unsigned int ResultValue;
}__attribute__((__packed__));


/******************  PostResult  *********************/
struct Fream_PostResult_eap {
	unsigned short SequenceID;
	unsigned short EventCode;
	unsigned int ResultCode;
	unsigned char SerialNumber[30];
}__attribute__((__packed__));

struct Fream_PostResult_plc {
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
	std::string  flag;
};

class IEventUpdater
{
public:
	virtual void UpdateEvent(std::string sectionName,std::string machineName,std::string eventName,unsigned char* data,unsigned int len) = 0;
};

class ev_reciver : public IEventUpdater
{
public:
	virtual ~ev_reciver();

	static ev_reciver& GetInstance();
	virtual void UpdateEvent(std::string sectionName,std::string machineName,std::string eventName,unsigned char* data,unsigned int len);

	friend class Director;
private:
	ev_reciver();

	TcpClient* m_client;
};

#define EV_DATA_BUFF_LEN 50

class Event {
public:
	Event()
		: m_evUpdater(ev_reciver::GetInstance())
		, m_machineContex(NULL)
	{
		memset(m_lastData,0,EV_DATA_BUFF_LEN);
	}

	Event(struct EvPara* ev_para)
		: m_evUpdater(ev_reciver::GetInstance())
	{
		memset(m_lastData,0,EV_DATA_BUFF_LEN);
		EV_PARAM_INIT(ev_para)
	}

	virtual ~Event(){}

	virtual void SniffingPlcEvent() = 0;
	virtual void SendEapData(unsigned char* data) = 0;

	void SetMachineContex(MachineContex* mc)
	{
		m_machineContex = mc;
	}

protected:

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

	char m_lastData[EV_DATA_BUFF_LEN];
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
	virtual void SendEapData(unsigned char* data);
	virtual void SniffingPlcEvent();
};


class Ev_EapCommand : public Event
{
public:
	Ev_EapCommand(struct EvPara* ev_para)
	{
		EV_PARAM_INIT(ev_para)
	}
	virtual ~Ev_EapCommand(){}
	virtual void SendEapData(unsigned char* data);
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
	virtual void SendEapData(unsigned char* data);
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
	virtual void SendEapData(unsigned char* data);
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
	virtual void SendEapData(unsigned char* data);
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
	virtual void SendEapData(unsigned char* data);
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
	virtual void SendEapData(unsigned char* data);
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
	virtual void SendEapData(unsigned char* data);
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
	virtual void SendEapData(unsigned char* data);
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
	virtual void SendEapData(unsigned char* data);
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
	virtual void SendEapData(unsigned char* data);
	virtual void SniffingPlcEvent();
};


#endif
