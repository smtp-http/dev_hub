#include "PlcProxy.h"


PlcProxy::PlcProxy(Protocol* p,PlcContex* pc)
	: m_protocol(p)
	, m_plcContex(pc)
{

}


PlcProxy::~PlcProxy()
{
	
}


int PlcProxy::PlcConnect()
{
	
}


void PlcProxy::PlcWriteWorlds(char* data,unsigned int long)
{

}


unsigned int PlcProxy::PlcReadWorlds(char* recvBuf)
{

}