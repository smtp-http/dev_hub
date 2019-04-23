#ifndef __PLC_PROXY_H__
#define __PLC_PROXY_H__

#include <string>

class PlcProxy;
/////////////////////////// PclContex /////////////////////////

class MachineContex
{
public:
	MachineContex(){}
	virtual ~MachineContex(){}

	void SetConnectionContex(void *contex){m_connectionContex = contex;}  // for example: struct fins_sys_tp *m_sys;
	std::string SectionName(){return m_sectionName;}
	std::string MachineName(){return m_machineName;}

	PlcProxy* GetProxy(){return m_proxy;}

protected:
	void* m_connectionContex;

	std::string m_ip;
	unsigned int m_port;

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
class PlcProxy
{
public:
	PlcProxy();
	virtual ~PlcProxy();

	virtual void on_disconnect(MachineContex*) = 0;
	virtual int PlcConnect(MachineContex*) = 0;
	virtual void PlcWriteWorlds(MachineContex*,char* data,unsigned int) = 0;
	virtual unsigned int PlcReadWorlds(MachineContex*,std::string plcAddr,char* recvBuf) = 0;

private:
	int m_connectionStatus;
	
};

class FinsPlcProxy : public PlcProxy
{
public:
	FinsPlcProxy(){}
	virtual ~FinsPlcProxy(){}

	virtual void on_disconnect(MachineContex*);
	virtual int PlcConnect(MachineContex*);
	virtual void PlcWriteWorlds(MachineContex*,char* data,unsigned int);
	virtual unsigned int PlcReadWorlds(MachineContex*,std::string plcAddr,char* recvBuf);
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////
PlcProxy* GetPlcProxy(std::string name);


#endif