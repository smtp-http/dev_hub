#ifndef __ELEC_BALANCE_H__
#define __ELEC_BALANCE_H__

//#include <sys/time.h>
#include "IEquipment.h"
#include "connbase.h"


namespace lux {
class ElecBalance : public Equipment
{
public:
	ElecBalance(int devNum,IUpdateSink *updater);//{m_devNumber = devNum;m_updater = updater;}
	virtual int OnData(char *,int);
	virtual int RecvCommand(const std::string cmd);
	virtual void OnTaskTimeout();
	
};

};




#endif
