
#include "ev_respond.h"



///////////////////////////////// ev_reciver ////////////////////////////

ev_reciver::ev_reciver()
{

}

ev_reciver::~ev_reciver()
{

}

ev_reciver& ev_reciver::GetInstance()
{
	static ev_reciver _instance;
	return _instance;
}

void ev_reciver::UpdateEvent(std::string sectionName,std::string machineName,std::string eventName)
{

}