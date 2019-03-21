#ifndef __USB_H__
#define __USB_H__

#include "Peripheral.h"
#include <string>

namespace lux {
class Usb : public Peripheral
{
public:
	Usb(std::string devName);
	~Usb();
	virtual int OpenPeripheral();
	//virtual Peripheral *Build(const string info)
//	void OnRead(int fd);
	//virtual void Close(int);
	//virtual void SetUpdater(IUpdateSink *updater);
	//virtual void SetEquipment(IEquipment *equ);

};

};

#endif
