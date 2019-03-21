#ifndef __TASK_H__
#define __TASK_H__

#include <iostream>
#include <string>
#include <map>
#include "eventlooper.h"
#include "connbase.h"


class Task : public lux::ITaskUse,public lux::ITimerUserSink
{
public:
	Task(lux::IMsgSender *sender);
	~Task();
	virtual lux::TaskStatus GetTaskStatus();
	void SetStatus(lux::TaskStatus status);
	virtual void AddTranscation(unsigned int tsId);
	virtual void Update(const std::string data,unsigned int transId);
	//virtual void UpdateInfo(const char *buf,int len) {std::cout << "---------update info-----------" << std::endl;}


	void SetReqId(int reqId);
	void SetCmdName(const std::string name);

	int GetReqId();
	std::string GetCmdName();

	void OnTimer(lux::TimerID tid);

	void StartResponseTimer(int val);
	void StopResponseTimer();

	virtual void SetAuxiliaryParam(const std::string MachineId,const std::string stationId,const std::string slotId);

	virtual void SetCommFormat(lux::CommunicationFormat f) {m_format = f;}

	lux::CommunicationFormat m_format;
	std::string m_MachineId;
	std::string m_stationId;
	std::string m_slotId;
	
protected:
	void SetTransStatus(unsigned int tsId);
	bool CheckTransStatusAll();
	void ResetAllTransStatus();
private:
	lux::IMsgSender *m_msgSender;
	lux::TaskStatus m_status;
	std::map<int,bool> m_transcationReptStatus;
	std::string m_cmdName;
	int m_reqId;
	lux::TimerID m_responseTimeout;
	
};




#endif

