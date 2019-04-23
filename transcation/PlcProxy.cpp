#include "PlcProxy.h"

using namespace std;


PlcProxy* GetPlcProxy(string name)
{
	FinsPlcProxy *fp;

	if(name == "Fins") {
		FinsPlcProxy* fp = new FinsPlcProxy;
		return fp;
	} else {
		return NULL;
	}
}

/////////////////////////////////////////////  PlcContex  //////////////////////////////////////////
PlcContex::PlcContex(std::string protoName,std::string sectionName,std::string machineName,std::string ip,unsigned int port)
{
	m_protocolType = protoName;
	m_ip = ip;
	m_port = port;
	m_proxy = GetPlcProxy(protoName);

	m_sectionName = sectionName;
	m_machineName = machineName;
}


//////////////////////////////////////////////   ////////////////////////////////////////

PlcProxy::PlcProxy()
{

}


PlcProxy::~PlcProxy()
{

}


//======================== FinsPlcProxy ===============================

void FinsPlcProxy::on_disconnect(MachineContex* mc)
{

}


int FinsPlcProxy::PlcConnect(MachineContex* mc)
{

}


void FinsPlcProxy::PlcWriteWorlds(MachineContex* mc,char* data,unsigned int len)
{

}


unsigned int FinsPlcProxy::PlcReadWorlds(MachineContex* mc,std::string plcAddr,char* recvBuf)
{

}


