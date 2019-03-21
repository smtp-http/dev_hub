#ifndef __UART_H__
#define __UART_H__

#include <string>
#include "Peripheral.h"


namespace lux {

class Uart : public Peripheral
{
public:
	Uart(std::string devName,int BaudRate);
	Uart(std::string devName);
	~Uart();
	int OpenPeripheral();
	//void SetUpdater(IUpdateSink *updater);
	//virtual void RemoveEquipment();
	//virtual void SetEquipment(IEquipment *equ);
	//virtual void Close();
	//void OnRead(int fd);
	//virtual Peripheral *Build(const string info);
	void SetBaudRate(int baudRate);
	
private:
	int m_baudRate;
	void SetSpeed(int fd, int speed);
	int SetParity(int fd,int databits,int stopbits,int parity);
};



};



#endif
