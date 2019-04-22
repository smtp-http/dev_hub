#ifndef __EV_RESPOND_H__
#define __EV_RESPOND_H__

#include <string>
#include <string.h>
#include "TcpClient.h"
#include "PlcProxy.h"

#define EV_PARAM std::string sectionName, \
		std::string machineName, \
		std::string eventName, \
		std::string plcEventAddr,unsigned int plcDataSize, \
		std::string eapEventAddr,unsigned int eapDataSize, \
		std::string flag

#define EV_PARAM_INIT m_sectionName = sectionName; \
		m_machineName = machineName; \
		m_eventName = eventName; \
		m_plcEventAddr = plcEventAddr; \
		m_plcDataSize = plcDataSize; \
		m_eapEventAddr = eapEventAddr; \
		m_eapDataSize = eapDataSize; \
		m_flag = flag;

class IEventUpdater
{
public:
	virtual void UpdateEvent(std::string sectionName,std::string machineName,std::string eventName,char* data,unsigned int len) = 0;
};

class ev_reciver : public IEventUpdater
{
public:
	virtual ~ev_reciver();

	static ev_reciver& GetInstance();
	virtual void UpdateEvent(std::string sectionName,std::string machineName,std::string eventName,char* data,unsigned int len);

private:
	ev_reciver();

	TcpClient* m_client;
};

#define EV_DATA_BUFF_LEN 50

class Event {
public:
	Event()
		: m_evUpdater(&ev_reciver::GetInstance())
		, m_machineContex(NULL)
	{
		memset(m_lastData,0,EV_DATA_BUFF_LEN);
	}

	virtual ~Event(){}

	void SniffingPlcEvent();
	virtual void SendEapData(char* data) = 0;

	void SetMachineContex(MachineContex* mc)
	{
		m_machineContex = mc;
	}

protected:
	std::string m_sectionName;
	std::string m_machineName;
	std::string m_eventName;

	std::string m_flag;     // "DM"

	std::string m_plcEventAddr;
	unsigned int m_plcDataSize;    // word

	std::string m_eapEventAddr;      // 
	unsigned int m_eapDataSize;

	IEventUpdater *m_evUpdater;

	MachineContex *m_machineContex;

	char m_lastData[EV_DATA_BUFF_LEN];
};


class Ev_Register : public Event
{
public:
	Ev_Register(EV_PARAM)
	{
		EV_PARAM_INIT
	}

	virtual ~Ev_Register(){}

	virtual void SendEapData(char* data);
};



#endif
