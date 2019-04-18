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
	ev_reciver();
	~ev_reciver();

	static ev_reciver& GetInstance();
	virtual void UpdateEvent(std::string sectionName,std::string machineName,std::string eventName);

private:
	TcpClient* m_client;
};




#endif
