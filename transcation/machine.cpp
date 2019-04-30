
#include <iostream>
#include <stdio.h>
#include "machine.h"
#include "station.h"
#include "config.h"


using namespace std;



Event* BuildEvent(string action,struct EvPara* ev_para)
{
	Event* ev = NULL;
	cout << "===  event action:" << ev_para->eventAction << endl;
	if (ev_para->eventAction == "Register")
		ev = new Ev_Register(ev_para);
	else if (ev_para->eventAction == "HeartBeat")
		ev = new Ev_HeareBeat(ev_para);
	else if (ev_para->eventAction == "EapCommand")
		ev = new Ev_EapCommand(ev_para);
	else if (ev_para->eventAction == "EventChangedFlag")
		ev = new Ev_EventChangedFlag(ev_para);
	else if (ev_para->eventAction == "NoticeText")
		ev = new Ev_NoticeMessage(ev_para);
	else if (ev_para->eventAction == "AlarmCode")
		ev = new Ev_AlarmCode(ev_para);
	else if (ev_para->eventAction == "MachineStatus")
		ev = new Ev_MachineStatus(ev_para);
	else if (ev_para->eventAction == "MachineYield")
		ev = new Ev_MachineYield(ev_para);
	else if (ev_para->eventAction == "BreakPoint")
		ev = new Ev_BreakPoint(ev_para);
	else
		printf("%s:%d  Event action name is not exist!\n",__FILE__,__LINE__);

	//cout << "=== ev:" << ev << endl;
	return ev;
}



//////////////////////////////////////////////////////// Machine //////////////////////////////////////////////////////////
Machine::Machine()
	: m_evUpdater(ev_reciver::GetInstance())
{
	struct timeval tv={0, SysConfig::Instance().GetPollingCycle()};
	m_valuePolling = EventLooper::GetInstance().ScheduleTimer(&tv, TF_FIRE_PERIODICALLY, this);

	unsigned int hb = (SysConfig::Instance().GetPollingCycle()*10)/1000;

	struct timeval tv1={hb,0};

	m_heartbeat = EventLooper::GetInstance().ScheduleTimer(&tv1, TF_FIRE_PERIODICALLY, this);
}

Machine::~Machine()
{
	EventLooper::GetInstance().CancelTimer(m_valuePolling);
	EventLooper::GetInstance().CancelTimer(m_heartbeat);
}

void Machine::EventPolling()
{
	map<string,Event*>::iterator it; 

    for(it=m_mainEvents.begin();it!=m_mainEvents.end();++it) {
    	it->second->SniffingPlcEvent();
    }

    for(it=m_stationsEvents.begin();it!=m_stationsEvents.end();++it) {
    	it->second->SniffingPlcEvent();
    }
}

Event* Machine::GetEvent(string evName)
{
	map<string,Event*>::iterator it; 

    for(it=m_mainEvents.begin();it!=m_mainEvents.end();++it) {
    	if(it->first == evName) {
    		return it->second;
    	}
    }

    for(it=m_stationsEvents.begin();it!=m_stationsEvents.end();++it) {
    	if(it->first == evName) {
    		return it->second;
    	}
    }
}


void Machine::PushMainEvent(string name,Event* ev)
{
	if (!m_mainEvents.count(name))
		m_mainEvents.insert (make_pair (name,ev));
}

void Machine::PushStationsEvent(string name,Event* ev)
{
	if (!m_stationsEvents.count(name))
		m_stationsEvents.insert (make_pair (name,ev));
}

void Machine::OnTimer(TimerID tid)
{
	// TODO: event polling
	if (tid == m_valuePolling) {
		EventPolling();
	} else if(tid == m_heartbeat) {
		Heartbeat();
	}
}



//=================================

PlcMachine::PlcMachine()
{
	ev_reciver& er = ev_reciver::GetInstance();
	this->SetEvUpdater(er);
}

PlcMachine::~PlcMachine()
{
	
}

void PlcMachine::Heartbeat()
{
	PlcProxy* proxy = m_contex->GetProxy();

}


////////////////////////////////////////////////////  Builder //////////////////////////////////////////////////////////////



//==============================

void MachinePlcBuilder::BuildMachine(string sectionName,LineMachine_t* lineMachine)
{
	MainDeviceProfile_t* mainDevPro = &lineMachine->mainDeviceProfile;

	m_machine = new PlcMachine;

	MachineContex* contex = new PlcContex("Fins",sectionName,lineMachine->Name,mainDevPro->IpAddress,mainDevPro->Port);
	m_machine->SetMachineContex(contex);

	// connect to plc
	PlcProxy* proxy = contex->GetProxy();
	if (proxy == NULL){
		printf("%s:%d  proxy is null!\n",__FILE__,__LINE__);
		return;
	}

	proxy->PlcConnect(contex);
}

void MachinePlcBuilder::BuildMainDeviceProfile(MainDeviceProfile_t *mainDevPro)
{
	if (m_machine == NULL){
		printf("%s:%d  m_machine is null!\n",__FILE__,__LINE__);
		return;
	}

	

	for (vector<StationEventProfile_t*>::const_iterator iter = mainDevPro->mainEvents.begin();iter != mainDevPro->mainEvents.end();iter++) {
		StationEventProfile_t* sep = *iter;

		Event* ev = NULL;
		struct EvPara ev_para = 
		{
			.eventName = sep->Name,
			.eventAction = sep->Action,
			.plcEventAddr = sep->PlcBlockAddress,
			.plcDataSize = sep->PlcBlockSize,
			.eapEventAddr = sep->EapBlockAddress,
			.eapDataSize = sep->EapBlockSize,
			.flag = sep->Flag
		};

		cout << "build main:  ";
		ev = BuildEvent(sep->Action,&ev_para);
		if (ev == NULL){
			printf("%s:%d  BuildEvent  null!\n",__FILE__,__LINE__);
			continue;
		}

		ev->SetMachineContex(m_machine->GetMachineContex());

		m_machine->PushMainEvent(ev_para.eventName,ev);
	}
	
}



#define PARA_INPUT(sep)  sep->Name,sep->Action,sep->PlcBlockAddress,sep->PlcBlockSize, \
						sep->EapBlockAddress,sep->EapBlockSize,sep->Flag


void MachinePlcBuilder::BuildCustomEvents(vector<StationEventProfile_t*>* mainEvList)
{
	if (m_machine == NULL){
		printf("%s:%d  m_machine is null!\n",__FILE__,__LINE__);
		return;
	}

	for (vector<StationEventProfile_t*>::const_iterator iter = mainEvList->begin();iter != mainEvList->end();iter++) {
		StationEventProfile_t* sep = *iter;

		Event* ev = NULL;
		struct EvPara ev_para = 
		{
			.eventName = sep->Name,
			.eventAction = sep->Action,
			.plcEventAddr = sep->PlcBlockAddress,
			.plcDataSize = sep->PlcBlockSize,
			.eapEventAddr = sep->EapBlockAddress,
			.eapDataSize = sep->EapBlockSize,
			.flag = sep->Flag
		};

		cout << "build custom:  ";
		ev = BuildEvent(sep->Action,&ev_para);
		if (ev == NULL){
			printf("%s:%d  BuildEvent  null!\n",__FILE__,__LINE__);
			continue;
		}

		ev->SetMachineContex(m_machine->GetMachineContex());

		m_machine->PushStationsEvent(ev_para.eventName,ev);
	}

}

//////////////////////////////////////////////////// Director //////////////////////////////////////////////////////////////

Director::Director()
	: m_evReciver(&ev_reciver::GetInstance())
{
	m_evReciver->m_client->SetFrameRecver(this);
}

Director::~Director()
{
	
}

void Director::ConstructMachine(string sectionName,LineMachine_t* lineMachine)
{
	if (lineMachine == NULL) {
		printf("line machine NULL!\n");
		return;
	}

	m_builder->BuildMachine(sectionName,lineMachine);
	
	m_builder->BuildCustomEvents(lineMachine->p_mainEvents);
	m_builder->BuildMainDeviceProfile(&lineMachine->mainDeviceProfile);

	Machine* mc = m_builder->GetMachine();
	if(mc == NULL){
		printf("machine is null: 0x%x\n",mc);
		return;
	}
	
	m_machines[sectionName + mc->GetName()] = mc;
	m_builder->SetMachine(NULL);
}


void Director::MachinesPolling()
{
	map<string,Machine*>::iterator it;   

    for(it=m_machines.begin();it!=m_machines.end();++it) {
        //cout<<"key: "<<it->first <<" value: "<<it->second<<endl; 
        it->second->EventPolling();
    }
}


int Director::WriteMachineData(std::string sectionName,std::string machineName,std::string eventName,unsigned char *data)
{
	Machine *m = m_machines[sectionName + machineName];

	if(m == NULL) {
		return NO_SECTIONNAME_OR_MACINENAME;
	}

	Event *ev = m->GetEvent(eventName);

	ev->SendEapData(data);
} 

bool Director::OnFrame(string& frame)
{
	
}

///////////////////////////////////////////////////  MachineScheduler /////////////////////////////////////////////////////


MachineScheduler::MachineScheduler(string xmlFile)
{
	m_plcDirector = new PlcDirector(new MachinePlcBuilder());

	this->FlashLineMachineList();
}

MachineScheduler::~MachineScheduler()
{

}

MachineScheduler& MachineScheduler::GetInstance(string xmlFile)
{
	static  MachineScheduler instance_(xmlFile);

	return instance_; 
}



//void ShowVec(const vector<StationEventProfile_t*>& valList);
LineStationDesiginProfile_t* GetLineStationDesiginProfile(std::string xmlFile);

void MachineScheduler::FlashLineMachineList()
{
	LineStationDesiginProfile_t *ls = GetLineStationDesiginProfile(SysConfig::Instance().GetPluginLineDesignerXml());
	
	for (vector<LineSection_t*>::const_iterator iter_sec = ls->LineSections->begin();iter_sec != ls->LineSections->end();iter_sec++) {
		m_sections[(*iter_sec)->Name] = *iter_sec;
		vector<LineMachine_t*>* lm_list = (*iter_sec)->Machines;//GetLineMachineList(m_xmlFile);
		if(lm_list!=NULL){
			for (vector<LineMachine_t*>::const_iterator iter = lm_list->begin(); iter != lm_list->end(); iter++){
				LineMachine_t* lineMachine = *iter;
				MainDeviceProfile_t* mainDevPro = &lineMachine->mainDeviceProfile;
				if (mainDevPro->DeviceType == "PLC" && lineMachine->Enable == true) {
					m_plcDirector->ConstructMachine((*iter_sec)->Name,lineMachine);
				}
			}
		}
	}
}



