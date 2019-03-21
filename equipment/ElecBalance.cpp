#include <string.h>
#include <iterator>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <json/json.h>
#include "ElecBalance.h"
#include <iostream>

using namespace lux;
using namespace std;


TimerID m_balanceClTimer;

/*
vector<string> &split(const string &str, char delim, vector<string> &elems, bool skip_empty = true) {
    istringstream iss(str);
    for (string item; getline(iss, item, delim); )
        if (skip_empty && item.empty()) continue;
        else elems.push_back(item);
    return elems;
}
*/
void ElecBalance::OnTaskTimeout()
{

}


int ElecBalance::OnData(char *data, int len)
{
	cout << "read message: " <<  data << endl;
	Json::Value root;
	//Json::Value arrayObj; 
	
	if (data[0] != 'K') {
		cout << "error frame!" << endl;
		string ret = "errorFrame";
		m_updater->UpdateInfo(ret.c_str(),ret.length());
		return -__LINE__;
	}

	if ( data[1] == '*') {
		struct timeval tv1={1, 0};
		
		if(m_balanceClTimer == -1)
			m_balanceClTimer = EventLooper::GetInstance().ScheduleTimer(&tv1, TF_FIRE_PERIODICALLY, this);
		//root["steady"] = "false";
		//arrayObj.append("false");
		return 0;
	} else if ( data[1] == ' ') {
		
		root["steady"] = "true";
		EventLooper::GetInstance().CancelTimer(m_balanceClTimer);
		m_balanceClTimer = -1;
		//arrayObj.append("true");
	} else {
		cout << "error steady data!" << endl;
		EventLooper::GetInstance().CancelTimer(m_balanceClTimer);
		m_balanceClTimer = -1;
		string ret = "errorData";
		m_updater->UpdateInfo(ret.c_str(),ret.length());
		return -__LINE__;
	}

	struct timeval tv;
    gettimeofday(&tv,NULL);

	int timeUse = (tv.tv_sec*1000 + tv.tv_usec/1000) - (m_startTime.tv_sec*1000 + m_startTime.tv_usec/1000);

	root["timeUse"] = timeUse;

	if ( data[2] == '+') {
		root["symbol"] = "+";
		//arrayObj.append("+");
	} else if ( data[2] == '-') {
		root["symbol"] = "-";
		//arrayObj.append("-");
	} else {
		cout << "error symbol data!" << endl;
		string ret = "errorData";
		m_updater->UpdateInfo(ret.c_str(),ret.length());
		return -__LINE__;
	}

	char buf[10];
	memset(buf,0,10);
	int j = 0;
	for(int i = 0;i < 8;i ++) {
		if (i == 0 && !data[i + 3] == ' ') {
			buf[j] = data[i + 3];
			j ++;
		}

		buf[j] = data[i + 3];
		j ++;
	}

	string val = buf;
	root["value"] = val;
	//arrayObj.append(val);

	j = 0;
	memset(buf,0,10);
	for ( int i = 0; i < 3; i ++) {
		if ( data[i + 11] != ' ') {
			buf[j] = data[i + 11];
			j ++;
		}
	}

	string unit = buf;

	root["unit"] = unit;
	//arrayObj.append(unit);

	//root["param"] = arrayObj;
	

	string out = root.toStyledString();
	
	m_updater->UpdateInfo(out.c_str(),out.length());
}

ElecBalance::ElecBalance(int devNum,IUpdateSink *updater)
{
	m_devNumber = devNum;
	if(updater != NULL)
		m_updater = updater;
	m_balanceClTimer = -1;
}


int ElecBalance::RecvCommand(const string cmd)
{

	return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////


