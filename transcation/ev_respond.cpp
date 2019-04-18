
#include "ev_respond.h"

using namespace std;

///////////////////////////////// ev_reciver ////////////////////////////

ev_reciver::ev_reciver()
	: m_client(NULL)
{
	return;
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