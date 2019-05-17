#include "PlcProxy.h"
#include <iostream>
using namespace std;

extern "C"
{
#include "fins.h"
}


PlcProxy* GetPlcProxy(string name)
{
	if(name == "Fins") {
		FinsPlcProxy* fp = new FinsPlcProxy;
		return fp;
	} else if(name == "ModbusRtu") {
		ModbusRtuPlcProxy* mp = new ModbusRtuPlcProxy;
		return mp;
		
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

PlcContex::PlcContex(std::string protoName,std::string sectionName,std::string machineName,unsigned int moduleNum,SerialParameter_t* serial)
{
	m_proxy = GetPlcProxy(protoName);

	m_sectionName = sectionName;
	m_machineName = machineName;

	m_serial = serial;
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

//===================================== FinsPlcProxy =========================================

void FinsPlcProxy::OnTimer(TimerID tid)
{
	if (tid == m_reconnect) {
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
	cout << "******************* on disconnect" << endl; 
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



int FinsPlcProxy::PlcWriteWorlds(string dataAddr,unsigned char* data,unsigned int len)
{
	int ret;
	ret = finslib_memory_area_write_word((struct fins_sys_tp*)m_plcConnectHandle,dataAddr.c_str(),data,len);
	
	if(ret != FINS_RETVAL_SUCCESS) {
		printf("%s:%d  FINS Write words error! ret: %d\n",__FILE__,__LINE__,ret);
		on_disconnect(m_contex);
		return -1;
	} 

	return ret;
}




//( struct fins_sys_tp *sys, const char *start, unsigned char *data, size_t num_word );
int FinsPlcProxy::PlcReadWorlds(string plcAddr,unsigned char* recvBuf,unsigned int recvLen)
{
	recvLen = recvLen / 2;
	//cout << "+++ " << plcAddr << "  " << recvLen << endl;

	int ret = finslib_memory_area_read_word((struct fins_sys_tp*)m_plcConnectHandle,plcAddr.c_str(),recvBuf,recvLen);

	if(ret != FINS_RETVAL_SUCCESS) {
		printf("%s:%d  FINS Read words error! ret: %d\n",__FILE__,__LINE__,ret);
		on_disconnect(m_contex);
		return -1;
	} 

	return 0;
}




//===================================== ModbusRtuPlcProxy =========================================


void ModbusRtuPlcProxy::on_disconnect(MachineContex* contex)
{

}


int ModbusRtuPlcProxy::PlcConnect(MachineContex* contex)
{

}

int ModbusRtuPlcProxy::PlcWriteWorlds(string dataAddr,unsigned char* data,unsigned int len)
{

}


int ModbusRtuPlcProxy::PlcReadWorlds(string plcAddr,unsigned char* recvBuf,unsigned int recvLen)
{

}


void ModbusRtuPlcProxy::OnTimer(TimerID tid)
{

}




