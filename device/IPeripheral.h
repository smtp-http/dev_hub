#ifndef __I_PERIPHERAL_H__
#define __I_PERIPHERAL_H__

class IPeripheral {
public:
	virtual int OpenPeripheral() = 0;
	virtual void Close() = 0;
};


#endif
