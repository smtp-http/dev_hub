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
PlcContex::PlcContex(string protocolName,string ip,unsigned int port)
{
	m_protocolType = protocolName;
	m_ip = ip;
	m_port = port;
	m_protocol = GetProtocol(protocolName);
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

