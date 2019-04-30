#ifndef __EV_RESPOND_H__
#define __EV_RESPOND_H__

#include <string>
#include <string.h>
#include "TcpClient.h"
#include "PlcProxy.h"



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

	void SniffingPlcEvent();
	virtual void SendEapData(unsigned char* data);

	void SetMachineContex(MachineContex* mc)
	{
		m_machineContex = mc;
	}

protected:

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

/*
class Ev_Register : public Event
{
public:
	Ev_Register(struct EvPara* ev_para)
	{
		EV_PARAM_INIT(ev_para)
	}

	//Ev_Register();
	virtual ~Ev_Register(){}

//	virtual void SendEapData(unsigned char* data);
};


class Ev_EapCommand : public Event
{
public:
	Ev_EapCommand(struct EvPara* ev_para)
	{
		EV_PARAM_INIT(ev_para)
	}
	virtual ~Ev_EapCommand(){}
//	virtual void SendEapData(unsigned char* data);
};

*/

#endif
