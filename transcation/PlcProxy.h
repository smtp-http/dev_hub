#ifndef __PLC_PROXY_H__
#define __PLC_PROXY_H__

#include <string>
#include "eventlooper.h"

#define CONNECT_OK  0
#define CONNECT_NO  1

using namespace lux;

class PlcProxy;
/////////////////////////// PclContex /////////////////////////

class MachineContex
{
public:
	MachineContex(){}
	virtual ~MachineContex(){}

	//void SetConnectionContex(void *contex){m_connectionContex = contex;}  // for example: struct fins_sys_tp *m_sys;
	std::string SectionName(){return m_sectionName;}
	std::string MachineName(){return m_machineName;}

	PlcProxy* GetProxy(){return m_proxy;}


	std::string m_ip;
	unsigned int m_port;

protected:
	PlcProxy* m_proxy;
	std::string m_protocolType;

	std::string m_sectionName;
	std::string m_machineName;
};


class PlcContex : public MachineContex
{
public:
	PlcContex(std::string protoName,std::string sectionName,std::string machineName,std::string ip,unsigned int port);
	virtual ~PlcContex(){}


private:
	
};


//////////////////////////// PlcProxy //////////////////////////
class PlcProxy : public ITimerUserSink
{
public:
	PlcProxy();
	virtual ~PlcProxy();

	virtual void on_disconnect(MachineContex*) = 0;
	virtual int PlcConnect(MachineContex*) = 0;
	virtual int PlcWriteWorlds(std::string,unsigned char* data,unsigned int) = 0;
	virtual int PlcReadWorlds(std::string plcAddr,unsigned char* recvBuf,unsigned int recvLen) = 0;
	virtual int PlcSendHeartbeat() = 0;

protected:
	virtual void OnTimer(TimerID tid){}

	TimerID m_reconnect;

	int m_connectionStatus;
	void* m_plcConnectHandle;

	MachineContex* m_contex;
};

class FinsPlcProxy : public PlcProxy
{
public:
	FinsPlcProxy(){}
	virtual ~FinsPlcProxy(){}

	virtual void on_disconnect(MachineContex*);
	virtual int PlcConnect(MachineContex*);
	virtual int PlcWriteWorlds(std::string,unsigned char* data,unsigned int);
	virtual int PlcReadWorlds(std::string plcAddr,unsigned char* recvBuf,unsigned int recvLen);

	virtual void OnTimer(TimerID tid);
	virtual int PlcSendHeartbeat();
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////
PlcProxy* GetPlcProxy(std::string name);


#endif