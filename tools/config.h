#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <list>
#include <json/json.h>


namespace lux {
class IConfigUser {
public:
	virtual void Update(const Json::Value &value) = 0;
};


class SysConfig
{
public:
	int LoadConfig();
	int LoadUserConfig(IConfigUser &);
	virtual void AddUser(IConfigUser *u);
	virtual void DelUser(IConfigUser *u);
	static SysConfig& Instance();

	std::string GetUperComputerIp(){return uperComputerIp;}
	short GetUperComputerPort(){return uperComputerPort;}
	std::string GetPlcProtocol(){return plcProtocol;}
private:
	SysConfig(){}
	std::list<IConfigUser *> m_users;

	std::string uperComputerIp;
	short uperComputerPort;
	std::string plcProtocol;
};


};

#endif

