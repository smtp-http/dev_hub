#include <stdio.h>
#include<string.h>
#include <algorithm>
#include <memory>
#include <vector>
#include <iostream>
#include <sstream>

extern "C"
{
#include "ut_log.h"
}

#include "TcpServer.h"
#include "Transcation.h"
#include "Uart.h"
#include "SolderCtrl.h"
#include "ElecBalance.h"

#include "task.h"
//#include "Log4cxxWrapper.h"




using namespace lux;
using namespace std;

vector<string> &split(const string &str, char delim, vector<string> &elems, bool skip_empty = true);


string BuildResponse(int errorNo,string cmd,int reqId)
{
	Json::Value root; 
	
	root["cmd"] = cmd; 
	root["resultCode"] = errorNo;
	root["ReqId"] = reqId;

	return root.toStyledString();

}

string BuildCommResponse(int errorNo,Task *task,string cmd,int reqId)
{
	stringstream ss;
	
	ss << task->m_MachineId << "," << task->m_stationId << "," << task->m_slotId << "," << cmd << "," << errorNo << "," << reqId;
	
	return ss.str();

}

string BuildDataResponse(Json::Value data,string cmd,int reqId)
{
	Json::Value root;
	
	root["cmd"] = cmd; 
	root["resultCode"] = 0;
	root["result"] = data;//arrayObj; 
	root["ReqId"] = reqId;

	return root.toStyledString();

}
string BuildCommDataResponse(vector<string> &data,Task *task,string cmd,int reqId)
{

	stringstream ss;
	vector<string>::iterator iter;

	ss << task->m_MachineId << "," << task->m_stationId << "," << task->m_slotId  ;

	for (iter = data.begin();iter != data.end();iter++) {
			
		//cout << "++ item[  "   << " ]  =  " << *iter << endl;
		ss << "," << *iter ;
		
	}

	return ss.str();

}




Transcation::Transcation()
{
	m_task = NULL;
}

Transcation::~Transcation()
{

}


void Transcation::AddServer(IUpdateSink * server)
{
	if (server != NULL) {
		m_servers.push_back(server);
	}
}

void Transcation::RemoveServer(IUpdateSink * server)
{
	m_servers.erase(find(m_servers.begin(), m_servers.end(), server));
}

//typedef Json::Writer JsonWriter;  
//typedef Json::Reader JsonReader;  
typedef Json::Value  JsonValue;    


void GetMemberFromJson(vector<string> &data,Json::Value &v){
	
	JsonValue::Members mem = v.getMemberNames();  
	for (auto iter = mem.begin(); iter != mem.end(); iter++) {		   
		cout<<*iter<<"\t: ";	
		stringstream ss;
		if (v[*iter].type() == Json::objectValue) {			   
			cout<<endl; 			 
			cout << v[*iter];		  
		} else if (v[*iter].type() == Json::arrayValue) {			   
			cout<<endl; 			 
			auto cnt = v[*iter].size(); 			 
			for (auto i = 0; i < cnt; i++) {				   
				cout << v[*iter][i]; 			 
			}		   
		} else if (v[*iter].type() == Json::stringValue) {			   
			cout<<v[*iter].asString()<<endl;	
			data.push_back(v[*iter].asString());
		} else if (v[*iter].type() == Json::realValue) {			   
			cout<<v[*iter].asDouble()<<endl;		  
		} else if (v[*iter].type() == Json::uintValue) {	
			
			ss << v[*iter].asUInt();
			data.push_back(ss.str());
			cout<<v[*iter].asUInt()<<endl;			
		} else {			   
			cout<<v[*iter].asInt()<<endl;		
			ss << v[*iter].asInt();
			data.push_back(ss.str());
		}	   
	}

}


void Transcation::UpdateInfo(const char *buf, int len)
{
	Json::Value root;  
	Json::Reader reader;

	vector<string> data;
	//vector<string>::iterator iter;
 
	if(!reader.parse(buf, root)){  
        cout << "json parse fail!" << endl;
	} else {  
		//cout << root["param"].asArray() << endl;//print "UP000000"  
		Task *task = (Task *)m_task;
		GetMemberFromJson(data,root);
		
		string res;
		if (task->m_format == format_json) {
			res = BuildDataResponse(root,task->GetCmdName(),task->GetReqId());
		} else if (task->m_format == format_common) {
			res = BuildCommDataResponse(data,task,task->GetCmdName(),task->GetReqId());
		}

		m_task->Update(res,m_transcationId);
	}  
}

void Transcation::OnTaskTimeout()
{
	list<Peripheral *>::iterator iter;
	
	for (iter = m_Peripherals.begin();iter != m_Peripherals.end();iter ++) {
		Peripheral *p = *iter;
		p->OnTaskTimeout();
	}
}

void Transcation::SetTask(ITaskUse * task)
{
	if (task != NULL)
		m_task = task;
}
int Transcation::ProcessCmd(const Json::Value & value)
{

}

int Transcation::DownInfo(string &info)
{
	Json::Value value;
	Json::Value root;
	cout << "trans id: " << m_transcationId << " \n down info: " << info << endl;

	std::auto_ptr<Json::Reader> pJsonParser(new Json::Reader(Json::Features::strictMode()));

	if(!pJsonParser->parse(info,value)){

		vector<string> result;
		vector<string>::iterator iter;
		split(info, ',', result);

		int i = 0;
		string MachineId;
		string stationId;
		string slotId;
		for (iter = result.begin();iter != result.end();iter++) {
			
			cout << "++ item[ "  << i << " ]  =  " << *iter << endl;
			if (i == 0) {
				MachineId = *iter;
			} 

			if (i == 1) {
				stationId = *iter;
			}

			if ( i == 2) {
				slotId = *iter;

				m_task->SetAuxiliaryParam(MachineId,stationId,slotId);
			}

			if ( i == 3 ) {
				Json::Value arrayObj;
            	int equpNum = std::stoi(slotId);
				if (equpNum < 0 || equpNum > 6) {
					printf("error equip num! %s : %s : %d",__FILE__,__PRETTY_FUNCTION__,__LINE__);
					return -__LINE__;
				}

				if (*iter == "BalanceQp" || *iter == "BalanceCl") {
					arrayObj.append(equpNum);
					root["param"] = arrayObj;
				} else if(*iter == "GetSpitParam") {
					root["equipNum"] = equpNum;
				} else {
					printf("message command error! %s : %s : %d",__FILE__,__PRETTY_FUNCTION__,__LINE__);
					return -__LINE__;
				}

				root ["cmd"] = *iter;
				
			}

			if (i == 4) {
				int reqId = std::stoi(*iter);
				if(reqId < 0) {
					printf("error reqId num! %s : %s : %d",__FILE__,__PRETTY_FUNCTION__,__LINE__);
					return -__LINE__;
				}

				root["ReqId"] = reqId;
			}

			
			i ++;
		}
		
		//printf("message Decode error! %s : %s : %d",__FILE__,__PRETTY_FUNCTION__,__LINE__);
		//return -__LINE__;

		string strCmd = root.toStyledString();

		pJsonParser->parse(strCmd,value);

		m_task->SetCommFormat(format_common);
	}else {
		m_task->SetCommFormat(format_json);
	}

	if(value["cmd"].isNull() ||  !value["cmd"].isString()){
		printf("message Decode error! %s : %s : %d",__FILE__,__PRETTY_FUNCTION__,__LINE__);
		return -__LINE__;
	}

	string cmd = value["cmd"].asString();

	

	if(value["ReqId"].isNull() || !value["ReqId"].isInt()) {
		printf("message Decode error! %s : %s : %d",__FILE__,__PRETTY_FUNCTION__,__LINE__);
		return -__LINE__;
	}
	

	if (!CheckCommandName(cmd) ) {
		string data = "NothingToDo";
		m_task->Update(data,m_transcationId);
		printf("this cmd is not this transcation's responsibility! trans id: %d",m_transcationId);
		return 0;
	}

	int val;

	if (value["cmd"] == "BalanceCl") {
		val = 20;
	} else {
		val = 15;
	}

	Task *task = (Task *)m_task;
	task->StartResponseTimer(val);

	m_task->SetStatus(task_accept);
	ProcessCmd(value);
	
	return 0;
}

void Transcation::AddPeripheral(Peripheral *e)
{
	if ( e != NULL ) {
		m_Peripherals.push_back(e);
	}
}

void Transcation::AddIpWhite(const std::string ip)
{
	m_ipWhiteList.push_back(ip);

}

bool Transcation::CheckIp(string ip)
{
	bool ret = false;
	list<string>::iterator iter;
	cout << "ip: " << ip << endl;
	for (iter = m_ipWhiteList.begin();iter != m_ipWhiteList.end();iter ++) { 
		cout << "*iter: " << *iter;
		if(ip == *iter) {
			ret = true; 
			break;
		}
	}

	return ret;
}

void Transcation::AddCommandName(const string commandName)
{
	m_commandNames.push_back(commandName);
}

bool Transcation::CheckCommandName(string command)
{
	list<string>::iterator iter;

	for (iter = m_commandNames.begin();iter != m_commandNames.end();iter ++) {
		if ( *iter == command ) {
			return true;
		}
	}

	return false;
}

Peripheral *Transcation::SelectPeripherals(int equipNo,string transName)
{
	list<Peripheral *>::iterator iter;
	
	for (iter = m_Peripherals.begin();iter != m_Peripherals.end();iter ++) {
		Peripheral *p = *iter;
		if ( p->GetEquipmentName() == transName && p->GetEquipmentNo() == equipNo) {
			return p;
		}
	}

	return NULL;
}

void Transcation::SetName(const string name)
{
	m_transName = name;
}


string Transcation::GetName()
{
	return m_transName;
}
/////////////////////////////////  TranscationHandheldScanner  //////////////////////////////

TranscationHandheldScanner::TranscationHandheldScanner(unsigned int transId)
{
	m_transcationId = transId;
}

TranscationHandheldScanner::~TranscationHandheldScanner()
{

}



TranscationStationScanner::TranscationStationScanner(unsigned int transId)
{
	m_transcationId = transId;
}

TranscationStationScanner::~TranscationStationScanner()
{

}


//////////////////////////////// TranscationSolderCtrl //////////////////////////////////
TranscationSolderCtrl::TranscationSolderCtrl(unsigned int transId)
{
	m_transcationId = transId;
}


TranscationSolderCtrl::~TranscationSolderCtrl()
{

}

int TranscationSolderCtrl::ProcessCmd(const Json::Value & value)
{
	string response;
	int equipNo;

	cout << "process solder command!" << endl;
		
	if (value["cmd"].asString() == "GetSpitParam") {
		cout << " ++ GetSpitParam" << endl;
		if ( !value["param"].isNull() && !value["param"].isArray()) {
			cout << " param error!" << endl;
			//response = BuildResponse(ERROR_FORMAT,"GetSpitParam",value["ReqId"].asInt());
			Task* task = (Task *)m_task;
			if(task->m_format == format_json) {
				response = BuildResponse(ERROR_FORMAT,"GetSpitParam",value["ReqId"].asInt());
			} else if (task->m_format == format_common) {
				response = BuildCommResponse(ERROR_FORMAT,task,"GetSpitParam",value["ReqId"].asInt());
			}
			m_task->Update(response,m_transcationId);
			return -__LINE__;
		} else {
			equipNo = value["param"][0].asInt();
		}	
			
			
		cout << "equip no: " << equipNo << endl;
		Task *task = (Task *)m_task;
	
		task->SetReqId(value["ReqId"].asInt());
		task->SetCmdName("GetSpitParam");
	
		Peripheral *p = SelectPeripherals(equipNo,m_transName);
		if ( p == NULL ) {
			//response = BuildResponse(ERROR_PARAM,"GetSpitParam",value["ReqId"].asInt());
			Task* task = (Task *)m_task;
			if(task->m_format == format_json) {
				response = BuildResponse(ERROR_PARAM,"GetSpitParam",value["ReqId"].asInt());
			} else if (task->m_format == format_common) {
				response = BuildCommResponse(ERROR_PARAM,task,"GetSpitParam",value["ReqId"].asInt());
			}
			m_task->Update(response,m_transcationId);
			return -__LINE__;
		}
	
	
		cout << "start sequence: " << endl;
		string req = value["cmd"].asString();
		//p->WriteData(req);
		p->RecvCommand(req);
	
		task->SetStatus(task_run);
		
	}
	
	return 0;
}


////////////////////////////////// TranscationBalance ////////////////////////////

TranscationBalance::TranscationBalance(unsigned int transId)
{
	m_transcationId = transId;
}


TranscationBalance::~TranscationBalance()
{

}

int TranscationBalance::ProcessCmd(const Json::Value & value)
{
	cout << "process balance command!" << endl;
	string response;

	int equipNo;

	
	if (value["cmd"].asString() == "BalanceQp") {
		//cout << " ++ BalanceQp" << endl;
		if ( !value["param"].isNull() && !value["param"].isArray()) {
			//cout << " param error!" << endl;
			Task* task = (Task *)m_task;
			if(task->m_format == format_json) {
				response = BuildResponse(ERROR_FORMAT,"BalanceQp",value["ReqId"].asInt());
			} else if (task->m_format == format_common) {
				response = BuildCommResponse(ERROR_FORMAT,task,"BalanceQp",value["ReqId"].asInt());
			}
			m_task->Update(response,m_transcationId);
			return -__LINE__;
		} else {
			equipNo = value["param"][0].asInt();
		}	

		
		
		cout << "equip no: " << equipNo  << endl;


		Peripheral *p = SelectPeripherals(equipNo,m_transName);
		if ( p == NULL ) {
			cout << " peripheral is null!" << endl;
			//response = BuildResponse(ERROR_PARAM,"BalanceQp",value["ReqId"].asInt());
			Task* task = (Task *)m_task;
			if(task->m_format == format_json) {
				response = BuildResponse(ERROR_PARAM,"BalanceQp",value["ReqId"].asInt());
			} else if (task->m_format == format_common) {
				response = BuildCommResponse(ERROR_PARAM,task,"BalanceQp",value["ReqId"].asInt());
			}
			m_task->Update(response,m_transcationId);
			return -__LINE__;
		}

		string QPcmd = "%ET";  //去皮
		cout << "-------- fd:" << p->GetFd() << endl;
		if (p->GetFd() > 0) {
			p->WriteData(QPcmd);
		} else {
			//response = BuildResponse(ERROR_DEVOPEN,"BalanceQp",value["ReqId"].asInt());
			Task* task = (Task *)m_task;
			if(task->m_format == format_json) {
				response = BuildResponse(ERROR_DEVOPEN,"BalanceQp",value["ReqId"].asInt());
			} else if (task->m_format == format_common) {
				response = BuildCommResponse(ERROR_DEVOPEN,task,"BalanceQp",value["ReqId"].asInt());
			}
			m_task->Update(response,m_transcationId);
			return -__LINE__;
		}

		//response = BuildResponse(0,"BalanceQp",value["ReqId"].asInt());
		Task* task = (Task *)m_task;
		if(task->m_format == format_json) {
			response = BuildResponse(0,"BalanceQp",value["ReqId"].asInt());
		} else if (task->m_format == format_common) {
			response = BuildCommResponse(0,task,"BalanceQp",value["ReqId"].asInt());
		}
		cout << "response    " << endl;
		m_task->Update(response,m_transcationId);
	} else if (value["cmd"].asString() == "BalanceCl") {
		if ( !value["param"].isNull() && !value["param"].isArray()) {
			//response = BuildResponse(ERROR_FORMAT,"BalanceQp",value["ReqId"].asInt());
			Task* task = (Task *)m_task;
			if(task->m_format == format_json) {
				response = BuildResponse(ERROR_FORMAT,"BalanceQp",value["ReqId"].asInt());
			} else if (task->m_format == format_common) {
				response = BuildCommResponse(ERROR_FORMAT,task,"BalanceQp",value["ReqId"].asInt());
			}
			m_task->Update(response,m_transcationId);
			return -__LINE__;
		} else {
			equipNo = value["param"][0].asInt();
		}	

		
		
		cout << "equip no: " << equipNo << endl;
		Task *task = (Task *)m_task;

		task->SetReqId(value["ReqId"].asInt());
		task->SetCmdName("BalanceCl");

		Peripheral *p = SelectPeripherals(equipNo,m_transName);
		if ( p == NULL ) {
			//response = BuildResponse(ERROR_PARAM,"BalanceCl",value["ReqId"].asInt());
			Task* task = (Task *)m_task;
			if(task->m_format == format_json) {
				response = BuildResponse(ERROR_PARAM,"BalanceQp",value["ReqId"].asInt());
			} else if (task->m_format == format_common) {
				response = BuildCommResponse(ERROR_PARAM,task,"BalanceQp",value["ReqId"].asInt());
			}
			m_task->Update(response,m_transcationId);
			return -__LINE__;
		}

		string CLcmd = "%ER";  //读天平数

		if ( p->GetFd() > 0) {
			cout << "write clcmd: " << CLcmd << endl;
			p->MarkTime();
			p->WriteData(CLcmd);
		} else {
			//response = BuildResponse(ERROR_DEVOPEN,"BalanceCl",value["ReqId"].asInt());
			Task* task = (Task *)m_task;
			if(task->m_format == format_json) {
				response = BuildResponse(ERROR_DEVOPEN,"BalanceQp",value["ReqId"].asInt());
			} else if (task->m_format == format_common) {
				response = BuildCommResponse(ERROR_DEVOPEN,task,"BalanceQp",value["ReqId"].asInt());
			}
			m_task->Update(response,m_transcationId);
			return -__LINE__;
		}

		task->SetStatus(task_run);

		
	}
	return 0;
}



///////////////////////////////////// TranscationGlueDispenser /////////////////////// 

TranscationGlueDispenser::TranscationGlueDispenser(unsigned int transId)
{
	m_transcationId = transId;
}

TranscationGlueDispenser::~TranscationGlueDispenser()
{

	
}

int TranscationGlueDispenser::ProcessCmd(const Json::Value & value)
{
	string response;
	int equipNo;

	cout << "process glue command!" << endl;
		
	if (value["cmd"].asString() == "GetSpitParam") {
		cout << " ++ GetSpitParam" << endl;
		if ( !value["param"].isNull() && !value["param"].isArray()) {
			cout << " param error!" << endl;
			//response = BuildResponse(ERROR_FORMAT,"GetSpitParam",value["ReqId"].asInt());
			Task* task = (Task *)m_task;
			if(task->m_format == format_json) {
				response = BuildResponse(ERROR_FORMAT,"GetSpitParam",value["ReqId"].asInt());
			} else if (task->m_format == format_common) {
				response = BuildCommResponse(ERROR_FORMAT,task,"GetSpitParam",value["ReqId"].asInt());
			}
			m_task->Update(response,m_transcationId);
			return -__LINE__;
		} else {
			equipNo = value["param"][0].asInt();
		}	
			
			
		cout << "equip no: " << equipNo << endl;
		Task *task = (Task *)m_task;
	
		task->SetReqId(value["ReqId"].asInt());
		task->SetCmdName("GetSpitParam");
	
		Peripheral *p = SelectPeripherals(equipNo,m_transName);
		if ( p == NULL ) {
			//response = BuildResponse(ERROR_PARAM,"GetSpitParam",value["ReqId"].asInt());
			Task* task = (Task *)m_task;
			if(task->m_format == format_json) {
				response = BuildResponse(ERROR_PARAM,"GetSpitParam",value["ReqId"].asInt());
			} else if (task->m_format == format_common) {
				response = BuildCommResponse(ERROR_PARAM,task,"GetSpitParam",value["ReqId"].asInt());
			}
			m_task->Update(response,m_transcationId);
			return -__LINE__;
		}
	
	
		cout << "start sequence: " << endl;
		string req = value["cmd"].asString();
		//p->WriteData(req);
		p->RecvCommand(req);
	
		task->SetStatus(task_run);
		
	}
	
	return 0;
}



///////////////////////////////// Transcation Manager ///////////////////////////////////
TranscationManager::TranscationManager()
{

}

TranscationManager& TranscationManager::GetInstance()
{
	static  TranscationManager instance_;
	return instance_; 
}

bool TranscationManager::OnDownStream(unsigned int transId,string &frame)
{
	if (m_transcations.find(transId) != m_transcations.end()) {
		cout << " ++ trans id: " << transId  << "frame: " << frame << endl;
		m_transcations[transId]->DownInfo(frame);
	}
}

bool TranscationManager::AddTranscation(Transcation *tc)
{
	if (tc == NULL){
		printf("%s:%d    This transcation is NULL!",__FILE__,__LINE__);
		return false;
	}

	if(m_transcations.find(tc->GetId()) != m_transcations.end()) {
		printf("%s:%d    This id is already  exist!",__FILE__,__LINE__);
		return false;
	}

	m_transcations[tc->GetId()] = tc;

	return true;
}

Equipment *BuildElecBalance(int equipNum,IUpdateSink *updater);

Equipment *BuildSolderCtrl(int equipNum,IUpdateSink *updater);
Equipment *BuildGlueDispenser(int equipNum,IUpdateSink *updater);


CommServer *BuildTcpServer(const string ip,short port);

void TranscationManager::GetTransIds(list<int> &transIds,string ip)
{
	map<unsigned int,Transcation *>::iterator it;

	it = m_transcations.begin();

	while(it != m_transcations.end()) {
		Transcation *ts = it->second;
		if ( ts->CheckIp(ip)) {
			transIds.push_back(it->first);
		}
		it ++;         
	}
}

Transcation *TranscationManager::GetTranscation(const unsigned int id)
{
	if (m_transcations.find(id) != m_transcations.end()) {
		return m_transcations[id];
	} else {
		return NULL;
	}
}


void TranscationManager::SetTransTask(unsigned int transId, ITaskUse * task)
{
	if(!task) {
		cout << "trans task null!" << endl;
		return;
	}
	
	if (m_transcations.find(transId) != m_transcations.end()) {
		m_transcations[transId]->SetTask(task);
		return;
	}

	cout << " ++ trans id: " << transId  << "is not exit!" << endl;
}


// 一个transcation暂时只支持一个Task,也就是每个transcation支持一个ip，
// 但是一个task可以对应多个transcation,也就是transcation的ip可以是相同的
// 每个transcation所用到的外设不能冲突。
// 每个外设对应一个设备

void TranscationManager::Update(const Json::Value &value)
{
	//LoggerWrapper log= LoggerWrapper::GetInstance();

	if(value["serverPort"].isNull() || !value["serverPort"].isInt()) {
		cout << "server port is null or is not int type!" << endl;
		return;
	}
	
	
	CommServer *tcpSvr = BuildTcpServer("0.0.0.0",value["serverPort"].asInt());
	if (tcpSvr == NULL) {
			cerr << "server start failed." << endl;
			return;
	}

	tcpSvr->SetTransManager(this);
	tcpSvr->SetDownChannel(this);
	
	if(value["transcations"].isNull()) {
		cout << "transcation is null" << endl;
		return;
	}

	Json::Value arrayTranscation  = value["transcations"];

	for(unsigned int i = 0; i < arrayTranscation.size(); i++) {
		unsigned int transcationId;
		if ( !arrayTranscation[i]["transcationId"].isNull() && arrayTranscation[i]["transcationId"].isInt()) {
			transcationId = arrayTranscation[i]["transcationId"].asInt();
		} else {
			//log.Log(true,LOGGER_ERROR,"arrayTranscation[%d][\"transcationId\"] is null or is not int type, [%s][%s][%d]",i,__FILE__,__PRETTY_FUNCTION__,__LINE__);
			continue;
		}

		string transName;

		if ( !arrayTranscation[i]["name"].isNull() && arrayTranscation[i]["name"].isString()) {
			transName = arrayTranscation[i]["name"].asString();
		}

		Transcation *trans;

		if ( transName == "balance") {
			trans = new TranscationBalance(transcationId);
		} else if (transName == "solder") {
			trans = new TranscationSolderCtrl(transcationId);
		} else if (transName == "GlueDispenser") {
			trans = new TranscationGlueDispenser(transcationId);
		} else {
			
			continue;
		}

		trans->SetName(transName);
		
		Json::Value arrayCmdName = value[transName];
		if ( !arrayCmdName.isNull() && arrayCmdName.isArray()) {
			for (unsigned j = 0; j < arrayCmdName.size(); j ++ ) {
				string cmdName;
				if(!arrayCmdName[j].isNull() && arrayCmdName[j].isString()){
					cmdName = arrayCmdName[j].asString(); 
					trans->AddCommandName(cmdName);
				} else {
				printf("arrayCmdName[%d] is null or is not string type, [%s][%s][%d]",j,__FILE__,__PRETTY_FUNCTION__,__LINE__);
					continue;
				}
			}
		}
		
		Json::Value arrayPeripherals = arrayTranscation[i]["peripherals"];
		for(unsigned int j = 0; j < arrayPeripherals.size(); j ++) {
			string peripheralName;
			if(!arrayPeripherals[j].isNull() && arrayPeripherals[j].isString()){
				peripheralName = arrayPeripherals[j].asString(); 
			} else {
				//log.Log(true,LOGGER_ERROR,"arrayEquipment[%d] is null or is not string type, [%s][%s][%d]",j,__FILE__,__PRETTY_FUNCTION__,__LINE__);
				continue;
			}
			if (strncmp(peripheralName.c_str(),"uart",4) == 0) {
				Json::Value uartValue = value[peripheralName];
				
				string devName;
				if(!uartValue["devName"].isNull() && uartValue["devName"].isString()){
					devName = uartValue["devName"].asString();
				} else {
					//log.Log(true,LOGGER_ERROR,"value[%s] is null or is not string type, [%s][%s][%d]",peripheralName.c_str(),__FILE__,__PRETTY_FUNCTION__,__LINE__);
					continue;
				}
				
				int baudRate;
				if(!uartValue["BaudRate"].isNull() && uartValue["BaudRate"].isInt()) {
					baudRate = uartValue["BaudRate"].asInt();
				} else {
					//log.Log(true,LOGGER_ERROR,"value[%s][\"BaudRate\"] is null or is not string type, [%s][%s][%d]",peripheralName.c_str(),__FILE__,__PRETTY_FUNCTION__,__LINE__);
					continue;
				}
				Peripheral *uart = new Uart(devName,baudRate);

				string strTransName;
				if(!uartValue["transcationName"].isNull() && uartValue["transcationName"].isString()) {
					strTransName = uartValue["transcationName"].asString();
				} else {
					//log.Log(true,LOGGER_ERROR,"value[%s][\"equipment\"] is null or is not string type, [%s][%s][%d]",peripheralName.c_str(),__FILE__,__PRETTY_FUNCTION__,__LINE__);
					continue;
				}

				int equipNum;
				if(!uartValue["equipNum"].isNull() && uartValue["equipNum"].isInt()) {
					equipNum = uartValue["equipNum"].asInt();
				} else {
					//log.Log(true,LOGGER_ERROR,"value[%s][\"equipNum\"] is null or is not int type, [%s][%s][%d]",peripheralName.c_str(),__FILE__,__PRETTY_FUNCTION__,__LINE__);
					continue;
				}
				
				Equipment *equipment = NULL;

				cout << " =============== trans name: "  << strTransName << endl;
				
				if (strTransName == "solder") {
					equipment = BuildSolderCtrl(equipNum,trans);
				} else if ( strTransName == "balance" ) {
					equipment = BuildElecBalance(equipNum,trans);
				} else if (strTransName == "GlueDispenser") {
					cout << "==========================    glue +++++++++++++++++++++" << endl;
					equipment = BuildGlueDispenser(equipNum,trans);
					printf("-------- equitment: 0x%x\n",equipment);
				}

				uart->SetEquipment(equipment);

				equipment->SetPeripheral(uart);
				
				uart->OpenPeripheral();

				uart->SetEquipmentName(strTransName);

				uart->SetEquipmentNo(equipNum);
				
				trans->AddPeripheral(uart);
			} else if (strncmp(peripheralName.c_str(),"usb",3) == 0) {
				
			} else {
				printf("%s:%d    equip name:%s\n",__FILE__,__LINE__,peripheralName.c_str());
			}
		}

		Json::Value arrayIpWhiteList = arrayTranscation[i]["ipWhiteList"];

		for ( unsigned int j= 0;j < arrayTranscation.size(); j ++) {
			string ip = arrayIpWhiteList[j].asString();
			trans->AddIpWhite(ip);
		}

		trans->AddServer(tcpSvr);
		AddTranscation(trans);
	}

	
}



//////////////////////////////////////////////////////// test ////////////////////////////////////////////////////




