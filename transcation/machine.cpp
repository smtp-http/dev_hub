
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


//==============================

void MachinePlcBuilder::BuildMainDeviceProfile(MainDeviceProfile_t *mainDevPro)
{
	if (m_machine == NULL){
		m_machine = new PlcMachine;
	}

	//m_machine
}

void MachinePlcBuilder::BuildMainEvents(vector<StationEventProfile_t*>* mainEvList)
{
	if (m_machine == NULL){
		m_machine = new PlcMachine;
	}

	
}

//////////////////////////////////////////////////// Director //////////////////////////////////////////////////////////////

Director::Director()
{

}

Director::~Director()
{
	
}

void Director::ConstructMachine(LineMachine_t* lineMachine)
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
	
	m_machines[mc->GetName()] = mc;
	m_builder->SetMachine(NULL);
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
				m_plcDirector->ConstructMachine(lineMachine);
			}
		}
	}
}



