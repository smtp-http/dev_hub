#ifndef __SOLDER_CTRL_H__
#define __SOLDER_CTRL_H__

#include <string>
#include "IEquipment.h"
#include "Sequence.h"
#include "connbase.h"


namespace lux {

////// control code

#define _STX 0x02
#define _ETX 0x03
#define _EOT 0x04
#define _ENQ 0x05
#define _ACK 0x06

#define _CAN 0x18



class SolderCtrl : public Equipment
{
public:
	SolderCtrl(){m_type = SolderSequence;}
	SolderCtrl(int equipNum,IUpdateSink *updater);//{m_devNumber = devNum;m_updater = updater;}
	virtual int OnData(char *, int);
	virtual int RecvCommand(const std :: string cmd);

	int WritePeripheralData(const char *data,int len);
	virtual void OnTaskTimeout();

	//virtual int Submit(SequenceType type,unsigned char *data,int len);
	void UpStream(const char *,int len);

	SequenceType m_type;
protected:
	friend class SequenceState;
	virtual void ChangeState(SequenceState *s);
	SequenceState *m_state;
};

class GlueDispenser : public SolderCtrl
{
public:
	GlueDispenser(){m_type = GlueSequence;}
	GlueDispenser(int equipNum,IUpdateSink *updater);
	//virtual int OnData(char *, int);
	virtual int RecvCommand(const std :: string cmd);

	//virtual int Submit(SequenceType type,unsigned char *data,int len);
};

};

#endif

