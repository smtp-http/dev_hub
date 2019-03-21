#ifndef __TRANSCATION_H__
#define __TRANSCATION_H__

#include <list>
#include <map>
#include <string>
#include <json/json.h>
#include "connbase.h"
#include "Peripheral.h"
#include "config.h"
#include "task.h"

namespace lux {


class Transcation : public IUpdateSink
{
public:
	Transcation();
	~Transcation();
	void AddServer(IUpdateSink *server);
	void RemoveServer(IUpdateSink *server);
	void AddPeripheral(Peripheral *e);
	void AddIpWhite(const std::string ip);
	void AddCommandName(const std::string commandName);
	virtual void UpdateInfo(const char *buf,int len);
	//virtual void UpdateInfo(const std::string data,unsigned int transId){}
	
	virtual int DownInfo(std::string &info);
	unsigned int GetId(){return m_transcationId;}
	bool CheckIp(std::string ip);
	void SetTask(ITaskUse *task);

	void SetName(const std::string name);
	std::string GetName();

	void OnTaskTimeout();
	
protected:
	unsigned int m_transcationId;
	ITaskUse *m_task;
	//Task *m_taskn;
	std::string m_transName;
	
	virtual int ProcessCmd(const Json::Value &value);
	bool CheckCommandName(const std::string command);

	Peripheral *SelectPeripherals(int equipNo,std::string transName);
	
private:
	typedef std::list<IUpdateSink*> ServerList;
	ServerList m_servers;
	std::list<Peripheral *> m_Peripherals;
	std::list<std::string> m_ipWhiteList;
	std::list<string> m_commandNames;
	
};


class TranscationHandheldScanner : public Transcation
{
public:
	TranscationHandheldScanner(unsigned int transId);
	~TranscationHandheldScanner();

};

class TranscationStationScanner : public Transcation
{
public:
	TranscationStationScanner(unsigned int transId);
	~TranscationStationScanner();


};

class TranscationGlueDispenser;

class TranscationSolderCtrl : public Transcation
{
public:
	TranscationSolderCtrl(unsigned int transId);
	~TranscationSolderCtrl();
protected:
	virtual int ProcessCmd(const Json::Value &value);

};


class TranscationBalance : public Transcation
{
public:
	TranscationBalance(unsigned int transId);
	~TranscationBalance();
protected:
		virtual int ProcessCmd(const Json::Value &value);


};

class TranscationGlueDispenser : public Transcation
{
public:
	TranscationGlueDispenser(unsigned int transId);
	~TranscationGlueDispenser();
protected:
	virtual int ProcessCmd(const Json::Value &value);
};



///////////////////////////////////////////  Transcation Manager /////////////////////////////////////

class TranscationManager : public IDownChannel,public IConfigUser,public ITranscationOwner
{
public:
	static TranscationManager& GetInstance();
	virtual bool OnDownStream(unsigned int transId,std::string &frame);
	bool AddTranscation(Transcation *tc);
	virtual void Update(const Json::Value &value);
	virtual void GetTransIds(std::list<int> &transIds,std::string ip);
	virtual void SetTransTask(unsigned int id,ITaskUse *task);
	Transcation* GetTranscation(const unsigned int id);
private:
	TranscationManager();
	std::map<unsigned int,Transcation *> m_transcations;
};


};



#endif

