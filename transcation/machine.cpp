
#include <iostream>
#include <stdio.h>
#include "machine.h"
#include "station.h"
#include "config.h"


using namespace std;

//////////////////////////////////////////////////////// Machine //////////////////////////////////////////////////////////
Machine::Machine()
{

}

Machine::~Machine()
{

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


//=================================

PlcMachine::PlcMachine()
{
	
	ev_reciver& er = ev_reciver::GetInstance();
	this->SetEvUpdater(&er);

	
}

PlcMachine::~PlcMachine()
{
	
}

////////////////////////////////////////////////////  Builder //////////////////////////////////////////////////////////////


Protocol* GetProtocol(string name)
{
	static FinsProtocol fp;

	if(name == "Fins") {
		return &fp;
	} else {
		return NULL;
	}
}

//==============================

void MachinePlcBuilder::BuildMachine()
{

	m_machine = new PlcMachine;
}

void MachinePlcBuilder::BuildMainDeviceProfile(MainDeviceProfile_t *mainDevPro)
{
	if (m_machine == NULL){

		//m_machine = new PlcMachine;
		return;
	}

	//m_machine
}

void MachinePlcBuilder::BuildMainEvents(vector<StationEventProfile_t*>* mainEvList)
{
	if (m_machine == NULL){
		//m_machine = new PlcMachine;
	}

	
}

//////////////////////////////////////////////////// Director //////////////////////////////////////////////////////////////

Director::Director()
{

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
	
	m_builder->BuildMainEvents(lineMachine->p_mainEvents);
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


int Director::WriteMachineData(std::string sectionName,std::string machineName,std::string eventName,char *data,unsigned int len)
{
	Machine *m = m_machines[sectionName + machineName];
	if(m == NULL) {
		return NO_SECTIONNAME_OR_MACINENAME;
	}

	Event *ev = m->GetEvent(eventName);
} 


///////////////////////////////////////////////////  MachineScheduler /////////////////////////////////////////////////////


MachineScheduler::MachineScheduler(string xmlFile)
{
	m_plcDirector = new PlcDirector(new MachinePlcBuilder());
	SysConfig& config = SysConfig::Instance();

	config.GetUperComputerIp();
	config.GetUperComputerPort();

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
	LineStationDesiginProfile_t *ls = GetLineStationDesiginProfile("demotest.xml");
	
	for (vector<LineSection_t*>::const_iterator iter_sec = ls->LineSections->begin();iter_sec != ls->LineSections->end();iter_sec++) {
		m_sections[(*iter_sec)->Name] = *iter_sec;
		vector<LineMachine_t*>* lm_list = (*iter_sec)->Machines;//GetLineMachineList(m_xmlFile);
		if(lm_list!=NULL){
			for (vector<LineMachine_t*>::const_iterator iter = lm_list->begin(); iter != lm_list->end(); iter++){
				LineMachine_t* lineMachine = *iter;
				m_plcDirector->ConstructMachine((*iter_sec)->Name,lineMachine);
			}
		}
	}
}



