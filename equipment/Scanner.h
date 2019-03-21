#ifndef __SCANNER_H__
#define __SCANNER_H__

#include "IScanner.h"
#include "connbase.h"
#include "IEquipment.h"

namespace lux {

class KeyenceScanner : public Equipment
{
public:
	KeyenceScanner(){}
	KeyenceScanner(IUpdateSink *updater,std::string dev);
	~KeyenceScanner();

	int OnData(char *data,int len);
	virtual void OnTaskTimeout(){}
	//int GetMode();
private:
	
};


class HandhelScanner : public Equipment
{
public:
	HandhelScanner(){}
	HandhelScanner(IUpdateSink *updater,std::string dev);
	~HandhelScanner();

	int OnData(char *data,int len);
private:
	IConnectorAcceptor *m_acceptor;
};


};


#endif

