#include "PlcProxy.h"

using namespace std;


Protocol* GetProtocol(string name)
{
	static FinsProtocol fp;

	if(name == "Fins") {
		return &fp;
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
	m_protocol = GetProtocol(protoName);

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


PlcProxy& PlcProxy::Instance()
{
	static PlcProxy _instance;
	return _instance;
}


int PlcProxy::PlcConnect(MachineContex *pc)
{

}


void PlcProxy::PlcWriteWorlds(MachineContex *pc,char* data,unsigned int long)
{

}


unsigned int PlcProxy::PlcReadWorlds(MachineContex *pc,string plcAddr,char* recvBuf)
{

}

