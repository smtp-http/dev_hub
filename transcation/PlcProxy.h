#ifndef __PLC_PROXY_H__
#define __PLC_PROXY_H__

#include <string>
#include <map>
#include "station.h"
#include "eventlooper.h"

#define CONNECT_OK  0
#define CONNECT_NO  1



using namespace lux;

class PlcProxy;
/////////////////////////// PclContex /////////////////////////



class MachineContex
{
public:
	MachineContex()
		: WordSwap(false)
		, IntSwap(false)
		, LongSwap(false)
		, ReadSkipZero(false)
		, m_serial(NULL)
	{

	}
	virtual ~MachineContex(){}

	//void SetConnectionContex(void *contex){m_connectionContex = contex;}  // for example: struct fins_sys_tp *m_sys;
	std::string SectionName()
	{
		return m_sectionName;
	}
	std::string MachineName()
	{
		return m_machineName;
	}

	

	PlcProxy* GetProxy(){return m_proxy;}


	std::string m_ip;
	unsigned int m_port;

	unsigned int m_moduleNum;

	bool ReadSkipZero;
	bool WordSwap;
	bool IntSwap;
	bool LongSwap;

	std::map<unsigned int,PlcAlarmInfo_t*> PlcAlarmCodeList;

	SerialParameter_t* m_serial;


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
	PlcContex(std::string protoName,std::string sectionName,std::string machineName,unsigned int moduleNum,SerialParameter_t* serial);
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

	int GetConnectionStatus()
	{
		return m_connectionStatus;
	}

protected:
	virtual void OnTimer(TimerID tid);

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

};


class ModbusRtuPlcProxy : public PlcProxy
{
public:
	ModbusRtuPlcProxy(){}
	virtual ~ModbusRtuPlcProxy(){}

	virtual void on_disconnect(MachineContex*);
	virtual int PlcConnect(MachineContex*);
	virtual int PlcWriteWorlds(std::string,unsigned char* data,unsigned int);
	virtual int PlcReadWorlds(std::string plcAddr,unsigned char* recvBuf,unsigned int recvLen);

};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
PlcProxy* GetPlcProxy(std::string name);


#endif