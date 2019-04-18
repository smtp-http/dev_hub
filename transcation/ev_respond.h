#ifndef __EV_RESPOND_H__
#define __EV_RESPOND_H__

#include <string>
#include "TcpClient.h"

class IEventUpdater
{
public:
	virtual void UpdateEvent(std::string sectionName,std::string machineName,std::string eventName) = 0;
};

class ev_reciver : public IEventUpdater
{
public:
	~ev_reciver();

	static ev_reciver& GetInstance();
	virtual void UpdateEvent(std::string sectionName,std::string machineName,std::string eventName);

private:
	ev_reciver();

	TcpClient* m_client;
};


class Event {
public:
	Event(std::string sectionName,
		std::string machineName,
		std::string eventName,
		std::string plcEventAddr,unsigned int plcDataSize,
		std::string eapEventAddr,unsigned int eapDataSize)
		: m_sectionName(sectionName)
		, m_machineName(machineName)
		, m_eventName(eventName)
		, m_plcEventAddr(plcEventAddr)
		, m_plcDataSize(plcDataSize)
		, m_eapEventAddr(eapEventAddr)
		, m_eapDataSize(eapDataSize)
	{

	}

	~Event(){}

	virtual void OnPlcEvent() = 0;
	virtual void SendEapData(char *data) = 0;

private:
	std::string m_sectionName;
	std::string m_machineName;
	std::string m_eventName;

	std::string m_plcEventAddr;
	unsigned int m_plcDataSize;    // word

	std::string m_eapEventAddr;      // 
	unsigned int m_eapDataSize;
};


//class 



#endif
