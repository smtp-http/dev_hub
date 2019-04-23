#include "PlcProxy.h"
#include <iostream>
using namespace std;

extern "C"
{
#include "fins.h"
}


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
PlcContex::PlcContex(string protoName,string sectionName,string machineName,string ip,unsigned int port)
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
	m_connectionStatus = CONNECT_NO;
	m_plcConnectHandle = NULL;
	m_contex = NULL;

	struct timeval tv={5, 0};
	m_reconnect = EventLooper::GetInstance().ScheduleTimer(&tv, TF_FIRE_PERIODICALLY, this);
}


PlcProxy::~PlcProxy()
{
	EventLooper::GetInstance().CancelTimer(m_reconnect);
}

//======================== FinsPlcProxy ===============================

void FinsPlcProxy::OnTimer(TimerID tid)
{

	cout << "proxy OnTimer " << tid << endl;
	if (tid == m_reconnect) {
		cout << "m_reconnect_timer: " << this << endl;
		if ( m_plcConnectHandle == NULL) {
			cout << "m_reconnect" << endl;
			if(m_contex != NULL){
				PlcConnect(m_contex);
			}
		}
	}
}


void FinsPlcProxy::on_disconnect(MachineContex* mc)
{
	finslib_disconnect((struct fins_sys_tp*)m_plcConnectHandle);
	m_plcConnectHandle = NULL;
	m_connectionStatus = CONNECT_NO;
}


int FinsPlcProxy::PlcConnect(MachineContex* mc)
{

	int error_val;

	if (mc == NULL) {
		cout << "MachineContex is null!" << endl;
		return -1;
	}

	m_contex = mc;

	cout << "ip:" << mc->m_ip << "  port: " << mc->m_port << endl;

	if ( m_connectionStatus == CONNECT_OK) {
		return 0;
	}


	m_plcConnectHandle = finslib_tcp_connect(NULL, mc->m_ip.c_str(), mc->m_port, 0, 10, 0, 0, 1,0,&error_val, 6);
	if(m_plcConnectHandle == NULL || error_val != 0) {
		cout << "plc fins connect error!" << endl;
		m_plcConnectHandle = NULL;
		return -1;
	}

	cout << "sys: " << m_plcConnectHandle << endl;
	((struct fins_sys_tp*)m_plcConnectHandle)->plc_mode = FINS_MODE_CS;
	m_connectionStatus = CONNECT_OK;

	return 0;
}


void FinsPlcProxy::PlcWriteWorlds(MachineContex* mc,char* data,unsigned int len)
{

}


unsigned int FinsPlcProxy::PlcReadWorlds(MachineContex* mc,string plcAddr,char* recvBuf)
{

}


