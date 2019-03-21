#ifndef __CODE_PAIRING_H__
#define __CODE_PAIRING_H__

#include "connbase.h"


#define __SUCCEED			0
#define __ERROR_ERRORRESULT 1
#define __ERROR_NOPAIRING   2


struct Record {
	int IsUseful;
	long long LastTimeUs;
	//struct timeval tv;
	std::string Code;
};

class CodePairing : public lux::IUpdateSink
{
public:
	~CodePairing();
	virtual void   UpdateInfo(const char *buf,int len);
	//virtual void UpdateInfo(const std::string data,unsigned int transId){}
	static CodePairing& GetInstance();

	void SetTcpTransfer(lux::IUpdateSink *transfer);

private:
	CodePairing();
	lux::IUpdateSink *m_tcpTransfer;
	struct Record m_lastRecord;
};





#endif
