
#include <iostream>
#include "task.h"
#include <json/json.h>
#include "Transcation.h"

using namespace lux;
using namespace std;


extern TimerID m_balanceClTimer;

string BuildResponse(int errorNo,string cmd,int reqId);
string BuildCommResponse(int errorNo,Task *task,string cmd,int reqId);


TaskStatus Task::GetTaskStatus()
{
	return m_status;
}

Task::Task(IMsgSender *sender)
	: m_msgSender(sender)
	, m_responseTimeout(-1)
{
	m_status = task_idle;

}

Task::~Task()
{
	
}

void Task::StartResponseTimer(int val)
{
	cout << "========= start timer" << endl;
	struct timeval tv1={val, 0};
	m_responseTimeout = EventLooper::GetInstance().ScheduleTimer(&tv1, TF_FIRE_PERIODICALLY, this);
}

void Task::StopResponseTimer()
{
	cout << "========= stop timer" << endl;
	EventLooper::GetInstance().CancelTimer(m_responseTimeout);
}


void Task::Update(const string data, unsigned int transId)
{
	
	if ( data == "NothingToDo")	 {
		SetTransStatus(transId);
		
		if (CheckTransStatusAll()) {
			cout << "nothing to do!\n";
			
			string res;
			if (m_format == format_json) {
				res = BuildResponse(ERROR_COMMAND,m_cmdName,m_reqId);
			} else if (m_format == format_common) {
				res = BuildCommResponse(ERROR_COMMAND, this, m_cmdName, m_reqId);
			}
			m_msgSender->SendMessage(res);
			StopResponseTimer();
			ResetAllTransStatus();
			m_status = task_idle;
		} 

		return;
	}

	m_msgSender->SendMessage(data);
	StopResponseTimer();
	ResetAllTransStatus();
	m_status = task_idle;
}

void Task::AddTranscation(unsigned int tsId) //向transStatus map中添加一个成员，初始化为 bool = 0
{
	map<int,bool>::iterator iter;

	if(m_transcationReptStatus.find(tsId) != m_transcationReptStatus.end()) {
		printf("%s:%d    This TranscationSessions is already exist!",__FILE__,__LINE__);
		return;
	}
	
	m_transcationReptStatus[tsId] = false;
}


void Task::SetStatus(TaskStatus status) //更改任务状态
{
	m_status = status;
}

void Task::SetTransStatus(unsigned int tsId) // 如果收到一个transcation的返回为“NothingToDo”,置位位1，如果有结果返回，则全部清零，并把结果返回给客户端
{
	map<int,bool>::iterator iter;

	if(m_transcationReptStatus.find(tsId) == m_transcationReptStatus.end()) {
		printf("%s:%d    This TranscationSessions is not exist!",__FILE__,__LINE__);
		return;
	}

	m_transcationReptStatus[tsId] = true;
}

bool Task::CheckTransStatusAll()   // 如果全部返回 "NothingToDo",则给客户端返回“error command”
{
	bool ret = true;
	map<int,bool>::iterator iter;
	
	for (iter = m_transcationReptStatus.begin();iter != m_transcationReptStatus.end();iter++) { 
		ret = ret && iter->second;
	}

	// 只要有一个是false,返回false。否则返回true

	return ret;
}


void Task::ResetAllTransStatus() //m_transcationReptStatus 的value全部清零,key不变
{
	map<int,bool>::iterator iter;
		
	for (iter = m_transcationReptStatus.begin();iter != m_transcationReptStatus.end();iter++) { 
		m_transcationReptStatus[iter->first] = false;
	}

}

void Task::SetReqId(int reqId)
{
	m_reqId = reqId;
}

void Task::SetCmdName(const string name)
{
	m_cmdName = name;
}

int Task::GetReqId()
{
	return m_reqId;
}

string Task::GetCmdName()
{
	return m_cmdName;
}

void Task::OnTimer(TimerID tid)
{
	map<int,bool>::iterator iter;
	
	if ( tid == m_responseTimeout) {

		for (iter = m_transcationReptStatus.begin();iter != m_transcationReptStatus.end();iter++) { 
			Transcation *trans = TranscationManager::GetInstance().GetTranscation(iter->first);
			trans->OnTaskTimeout();
		}
	
		cout << "on timer" << endl;
		EventLooper::GetInstance().CancelTimer(m_balanceClTimer);
		m_balanceClTimer = -1;
		m_status = task_idle;
		ResetAllTransStatus();
		StopResponseTimer();
		Json::Value root;
		root["cmd"] = "Timeout";
		root["ReqId"] = m_reqId;
		
		string res = root.toStyledString();
		
		m_msgSender->SendMessage(res);
	}
}


void Task::SetAuxiliaryParam(const string MachineId, const string stationId, const string slotId)
{
	m_MachineId = MachineId;
	m_stationId = stationId;
	m_slotId = slotId;
}

///////////////////////////////////////////////////////////////////

ITaskUse *BuildTask(IMsgSender *msg)
{
	return new Task(msg);
}


