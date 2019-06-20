#include <string.h>
#include "config.h"
#include "autolock.h"

extern CHttpClient* GetHttpClient(string url);

AutoLockInfoProcess::AutoLockInfoProcess()
	: m_proxy(GetPlcProxy("fins"))
{
	if(SysConfig::Instance().UperProtocol == "tcp"){
		m_client = new TcpClient(SysConfig::Instance().GetUperComputerIp(),(short)SysConfig::Instance().GetUperComputerPort());
		//m_client->Connect();
	} else if (SysConfig::Instance().UperProtocol == "http") {
		m_client = GetHttpClient(SysConfig::Instance().UperUrl);
	} else {
		printf("%s:%d  UperProtocol is not define!\n",__FILE__,__LINE__);
		m_client = NULL;
	}

	struct timeval tv={0, 200};
	m_flash = EventLooper::GetInstance().ScheduleTimer(&tv, TF_FIRE_PERIODICALLY, this);
}

AutoLockInfoProcess::~AutoLockInfoProcess()
{
	if(m_client != NULL) {
		delete m_client;
	}

	if(m_proxy != NULL) {
		delete m_proxy;
	}
}


void AutoLockInfoProcess::UpdateEvent(const char* msg,unsigned int len)
{
	if(m_client != NULL)
		m_client->SendMsg(msg,len);
}

void AutoLockInfoProcess::OnTimer(TimerID tid)
{
	unsigned char result[100];
	memset(result,0,100);

	if (tid == m_flash) {

		cout << "m_flash" << endl;
		//m_proxy->PlcReadWorlds()

	}
}