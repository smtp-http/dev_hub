#ifndef __I_DEVICE_H__
#define __I_DEVICE_H__

#include <string>
#include "connbase.h"
#include <sys/time.h>
#include <vector>
#include "eventlooper.h"

namespace lux {

class IEquipment
{
public:
	virtual int OnData(char *,int) = 0;
	virtual int RecvCommand(const std::string cmd) = 0;
	virtual void TimeMark() = 0;
	virtual void OnTaskTimeout() = 0;
};


class Equipment : public IEquipment,public lux::ITimerUserSink
{
public:
	//virtual void EquipInit() = 0;
	virtual int OnData(char *,int);

	void SetPeripheral(IWritePeripheralData *p);
	void SetUpdater(IUpdateSink *updater);//{m_updater = updater;}
	void SetDevNumber(unsigned int devn);//{m_devNumber = devn;}
	
	virtual int RecvCommand(const std::string cmd);

	virtual void TimeMark();
	virtual void OnTimer(TimerID tid);
	virtual void OnTaskTimeout(){}
//	virtual int GetMode() = 0;

	
protected:
	std::string m_devName;
//	std::string m_devRole;
	unsigned int m_devNumber;
	IUpdateSink *m_updater;
	IWritePeripheralData *m_peripheral;
	struct timeval m_startTime;
	std::vector<char> m_equipData;
	//TimerID m_balanceClTimer;
};

};

#endif

