#ifndef __PERPIPHERAL_H__
#define __PERPIPHERAL_H__

#include <string>
#include <event2/event.h>
#include <event2/util.h>
#include <event2/buffer.h>
//#include <queue>
#include "IPeripheral.h"
#include "IScanner.h"
#include "connbase.h"
//#include "eventlooper.h"
#include "IEquipment.h"


namespace lux {

class Peripheral : public IPeripheral,public IWritePeripheralData
{
public:
	Peripheral();
	~Peripheral();
	//virtual Peripheral *Build(const string info) = 0;
	virtual int OpenPeripheral() = 0;
	void Close();
	int GetFd();
	virtual void OnRead(int fd);
	int WriteData(std::string &data);
	virtual int WriteData(const char *data,int len);
	virtual int RecvCommand(const std::string cmd);
	//virtual void SetUpdater(IUpdateSink *updater) = 0;
	virtual void SetEquipment(IEquipment *equ);
	virtual void RemoveEquipment();
	std::string GetDevName(){return m_devName;}

	void SetEquipmentName(const std::string name);
	void SetEquipmentNo(const int no);

	std::string GetEquipmentName();
	int GetEquipmentNo();

	void MarkTime();

	void OnTaskTimeout(){if(m_equipment != NULL)m_equipment->OnTaskTimeout();}
	
protected:

	int m_fd;
	struct event *m_rwEvent;
	struct event_base *m_evBase;
	std::string m_devName;
	char m_readBuffer[1024];
	IEquipment *m_equipment;
	bool m_writeBusy;
	std::string m_equipmentName;
	int m_equipmentNo;
};



};

#endif

