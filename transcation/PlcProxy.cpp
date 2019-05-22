#include "PlcProxy.h"
#include <iostream>
using namespace std;

extern "C"
{
#include "fins.h"
#include "modbus.h"
}

#include "config.h"

PlcProxy* GetPlcProxy(string name)
{
	if(name == "Fins") {
		FinsPlcProxy* fp = new FinsPlcProxy;
		return fp;
	} else if(name == "RTU") {
		ModbusRtuPlcProxy* mp = new ModbusRtuPlcProxy;
		return mp;
	} else if (name == "ModBus") {
		
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

	m_moduleNum = moduleNum;

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

void PlcProxy::OnTimer(TimerID tid)
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



//===================================== FinsPlcProxy =========================================


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


void ModbusRtuPlcProxy::on_disconnect(MachineContex* mc)
{
	cout << "******************* on disconnect" << endl; 
	// TODO:   modbus disconnect   finslib_disconnect((struct fins_sys_tp*)m_plcConnectHandle);
	m_plcConnectHandle = NULL;
	m_connectionStatus = CONNECT_NO;
}


int ModbusRtuPlcProxy::PlcConnect(MachineContex* mc)
{

	int error_val;

	if(mc->m_serial == NULL){
		printf("%s:%d  m_serial is null!\n",__FILE__,__LINE__);
		return -1;
	}

	if (mc == NULL) {
		cout << "MachineContex is null!" << endl;
		return -1;
	}

	m_contex = mc;

	cout << "m_moduleNum:" << mc->m_moduleNum << "  baud: " << mc->m_serial->baud  << "  parity: " << mc->m_serial->parity << "  data_bit: " << mc->m_serial->data_bit << "  stop_bit: " << mc->m_serial->stop_bit << endl;

	if ( m_connectionStatus == CONNECT_OK) {
		return 0;
	}


	m_plcConnectHandle = modbus_new_rtu(SysConfig::Instance().SerialModbusRtu.c_str(), mc->m_serial->baud,mc->m_serial->parity, mc->m_serial->data_bit, mc->m_serial->stop_bit);

	modbus_t *ctx = (modbus_t*)m_plcConnectHandle;
	modbus_set_debug(ctx, TRUE);
	modbus_set_error_recovery(ctx,
							(modbus_error_recovery_mode)(MODBUS_ERROR_RECOVERY_LINK | MODBUS_ERROR_RECOVERY_PROTOCOL));

	modbus_set_slave(ctx, mc->m_moduleNum);

}

int ModbusRtuPlcProxy::PlcWriteWorlds(string dataAddr,unsigned char* data,unsigned int len)
{
	// fins:dataAddr's flag is a string, like "DM".  for modbus rtu ,it is null string
	int addr;

	try{
		addr = stoi(dataAddr);
	} catch (std::exception &ex) {
		printf("%s:%d  dataAddr: %s is invalid_argument!\n",__FILE__,__LINE__,dataAddr.c_str());
		return -1;
	}

	modbus_t *ctx = (modbus_t*)m_plcConnectHandle;

	int ret = modbus_write_registers(ctx,addr,len,(uint16_t*)data);

	if (ret != 1) {
		printf("%s:%d  modbus_write_registers error! ret: %d\n",__FILE__,__LINE__,ret);
		return -1;
	}

	return 0;
}


int ModbusRtuPlcProxy::PlcReadWorlds(string plcAddr,unsigned char* recvBuf,unsigned int recvLen)
{
	int addr;

	try{
		addr = stoi(plcAddr);
	} catch (std::exception &ex) {
		printf("%s:%d  plcAddr: %s is invalid_argument!\n",__FILE__,__LINE__,plcAddr.c_str());
		return -1;
	}

	modbus_t *ctx = (modbus_t*)m_plcConnectHandle;

	int ret = modbus_read_registers(ctx,addr,recvLen,(uint16_t*)recvBuf);
	if(ret != recvLen){
		printf("%s:%d  modbus_read_registers error! ret: %d\n",__FILE__,__LINE__,ret);
		return -1;
	}

	return 0;
}





