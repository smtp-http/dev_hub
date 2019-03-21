#include "IEquipment.h"
#include <iostream>

using namespace lux;
using namespace std;


int Equipment::OnData(char *, int)
{

}

void Equipment::SetUpdater(IUpdateSink *updater)
{
	if (updater != NULL)
		m_updater = updater;
}

void Equipment::SetDevNumber(unsigned int devn)
{
	m_devNumber = devn;
}

int Equipment::RecvCommand(const string cmd)
{

}

void Equipment::SetPeripheral(IWritePeripheralData * p)
{
	if ( p != NULL) {
		m_peripheral = p;
	}
}

void Equipment::TimeMark()
{
	 gettimeofday(&m_startTime,NULL);
}

extern TimerID m_balanceClTimer;

void Equipment::OnTimer(TimerID tid)
{
	if(tid == m_balanceClTimer) {
		string CLcmd = "%ER";  //读天平数
		cout << " -cl--- ";
		
		m_peripheral->WriteData(CLcmd.data(),CLcmd.length());
	}
	
}




