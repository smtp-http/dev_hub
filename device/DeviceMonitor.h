#ifndef __DEVICE_MONITOR_H__
#define __DEVICE_MONITOR_H__

#include <sstream>
#include <map>
#include "IPeripheral.h"
#include "eventlooper.h"



class DeviceMonitor :  public lux::ITimerUserSink
{

public:
	DeviceMonitor();
	~DeviceMonitor();
	static DeviceMonitor& GetInstance();

	void OnTimer(lux::TimerID tid); 
private:
	std::map<std::string,IPeripheral*> m_peripherals;
	lux::TimerID m_devCheck;
};

#endif

