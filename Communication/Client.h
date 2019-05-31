#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <string>

class IFrameRecver
{
public:
	virtual bool OnFrame(std::string& frame) = 0;
};


class Client
{
protected:
	IFrameRecver* m_frameRecver;
public:
	Client()
		: m_frameRecver(NULL)
	{

	}
	void SetFrameRecver(IFrameRecver *fr)
	{
		m_frameRecver = fr;
	}

	virtual int SendMsg(const char* buf,unsigned int len) = 0;
	//virtual bool Connect(){}
};


#endif
