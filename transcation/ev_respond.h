#ifndef __EV_RESPOND_H__
#define __EV_RESPOND_H__

#include <string>
#include "TcpClient.h"

#define EV_PARAM std::string sectionName, \
		std::string machineName, \
		std::string eventName, \
		std::string plcEventAddr,unsigned int plcDataSize, \
		std::string eapEventAddr,unsigned int eapDataSize

#define EV_PARAM_INIT m_sectionName = sectionName; \
		m_machineName = machineName; \
		m_eventName = eventName; \
		m_plcEventAddr = plcEventAddr; \
		m_plcDataSize = plcDataSize; \
		m_eapEventAddr = eapEventAddr; \
		m_eapDataSize = eapDataSize;

class IEventUpdater
{
public:
	virtual void UpdateEvent(std::string sectionName,std::string machineName,std::string eventName,char* data) = 0;
};

class ev_reciver : public IEventUpdater
{
public:
	~ev_reciver();

	static ev_reciver& GetInstance();
	virtual void UpdateEvent(std::string sectionName,std::string machineName,std::string eventName,char* data);

private:
	ev_reciver();

	TcpClient* m_client;
};


class Event {
public:
	Event()
		: m_evUpdater(&ev_reciver::GetInstance())
	{}

	~Event(){}

	void OnPlcEvent(char* data);
	virtual void SendEapData(char* data) = 0;

protected:
	std::string m_sectionName;
	std::string m_machineName;
	std::string m_eventName;

	std::string m_plcEventAddr;
	unsigned int m_plcDataSize;    // word

	std::string m_eapEventAddr;      // 
	unsigned int m_eapDataSize;

	IEventUpdater *m_evUpdater;
};


class Ev_Register : public Event
{
public:
	Ev_Register(EV_PARAM)
	{
		EV_PARAM_INIT
	}

	~Ev_Register(){}

	//virtual void OnPlcEvent(char* data);
	virtual void SendEapData(char* data);
};



#endif
