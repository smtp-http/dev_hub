#ifndef __AUTO_LOCK_H__
#define __AUTO_LOCK_H__

#include "eventlooper.h"
#include "TcpClient.h"
#include "PlcProxy.h"
#include "ev_respond.h"

using namespace lux;



class AutoLockInfoProcess : public IEventUpdater,public ITimerUserSink {
public:
	AutoLockInfoProcess();
	~AutoLockInfoProcess();

	virtual void UpdateEvent(const char* msg,unsigned int len);
	virtual void OnTimer(TimerID tid);
private:
	Client* m_client;
	PlcProxy* m_proxy;
	TimerID m_flash;
};




#endif