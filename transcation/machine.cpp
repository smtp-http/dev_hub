
#include <iostream>
#include "machine.h"
#include "station.h"


using namespace std;

//////////////////////////////////////////////////////// Machine //////////////////////////////////////////////////////////
Machine::Machine()
{

}

Machine::~Machine()
{

}










PlcMachine::PlcMachine()
{

}

PlcMachine::~PlcMachine()
{
	
}

//////////////////////////////////////////////////// Director //////////////////////////////////////////////////////////////

Director::Director()
{

}

Director::~Director()
{
	
}


///////////////////////////////////////////////////  MachineScheduler /////////////////////////////////////////////////////


MachineScheduler::MachineScheduler(string xmlFile)
	: m_xmlFile(xmlFile)
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


void ShowVec(const vector<StationEventProfile_t*>& valList);
void MachineScheduler::FlashLineMachineList()
{
	vector<LineMachine_t*>* lm_list = GetLineMachineList(m_xmlFile);
	if(lm_list!=NULL){
		for (vector<LineMachine_t*>::const_iterator iter = lm_list->begin(); iter != lm_list->end(); iter++){
			cout << "================ main events ==================" << endl;
			ShowVec(*(*iter)->p_mainEvents); //main events

			for (vector<LineStation_t*>::const_iterator ils = (*iter)->Stations.begin(); ils != (*iter)->Stations.end(); ils++){
				cout << "+++++++++ station ++++++" << endl;
				ShowVec((*ils)->Events); //main events
			}

		}
	}
}



