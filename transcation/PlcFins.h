#ifndef __PLC_FINS_H__
#define __PLC_FINS_H__

#include "connbase.h"
#include "eventlooper.h"
#include "Peripheral.h"


/////////////////// status ///////////////
#define PLC_STATUS_BUSY 0
#define PLC_STATUS_IDEL 1



///////////////// cmd ////////////////////
#define EBALANCE_QP 1
#define EBALANCE_CZ 3

////////////////// error code ////////////
#define OK 0
#define ERROR_CMD 2
#define ERROR_TIMEOUT 4
#define ERROR_RESULT 6

#include <json/json.h>

extern "C"
{
#include "fins.h"
}
using namespace lux;


class PlcFins : public IUpdateSink,public ITimerUserSink {
public:
	PlcFins(std::string ip,int port,std::string cmd_addr,std::string data_addr);
	~PlcFins();

	int Connect();
	void SetPeripheral(Peripheral *e){m_Peripheral = e;}
	virtual void UpdateInfo(const char *buf,int len);

protected:
	virtual void OnTimer(TimerID tid);

	std::string m_ip;
	int m_port;
	std::string m_cmdAddr;
	std::string m_dataAddr;
	struct fins_sys_tp *m_sys;
	TimerID m_detect;
	TimerID m_reconnect;
	TimerID m_valuePolling;
	Peripheral *m_Peripheral;
private:
	void on_disconnect();
	int m_status;
	unsigned char m_cmd[5];
	unsigned char m_data[10];
	struct timeval m_startTime;
};



#endif
