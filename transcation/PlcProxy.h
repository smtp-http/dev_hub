#ifndef __PLC_PROXY_H__
#define __PLC_PROXY_H__

////////////////////////// Protocol /////////////////////////
class Protocol
{

};

class FinsProtocol : public Protocol
{

};

/////////////////////////// PclContex /////////////////////////


class PlcContex
{

};

class PlcContexFins : public PlcContex
{

};

//////////////////////////// PlcProxy //////////////////////////
class PlcProxy
{
public:
	PlcProxy(Protocol* ,PlcContex* );
	~PlcProxy();
	int PlcConnect();
	void PlcWriteWorlds(char* data,unsigned int long);
	unsigned int PlcReadWorlds(char* recvBuf);

private:
	Protocol* m_protocol;
	PlcContex* m_plcContex;
};





#endif