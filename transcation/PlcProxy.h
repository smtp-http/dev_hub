#ifndef __PLC_PROXY_H__
#define __PLC_PROXY_H__

#include <string>
////////////////////////// Protocol /////////////////////////
class Protocol
{

};

class FinsProtocol : public Protocol
{

};

/////////////////////////// PclContex /////////////////////////

class MachineContex
{
public:

protected:
	std::string protocol;
};


class PlcContex : public MachineContex
{
public:
	PlcContex(){}
	~PlcContex(){}

};


//////////////////////////// PlcProxy //////////////////////////
class PlcProxy
{
public:
	PlcProxy();
	~PlcProxy();
	int PlcConnect();
	void PlcWriteWorlds(char* data,unsigned int long);
	unsigned int PlcReadWorlds(std::string plcAddr,char* recvBuf);

private:
	
};





#endif