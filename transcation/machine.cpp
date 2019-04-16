
#include "machine.h"
#include "station.h"



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


MachineScheduler::MachineScheduler()
{
	m_plcDirector = new PlcDirector(new MachinePlcBuilder());
	this->FlashLineMachineList();
}

MachineScheduler::~MachineScheduler()
{

}

MachineScheduler& MachineScheduler::GetInstance()
{
	static  MachineScheduler instance_;

	return instance_; 
}

void MachineScheduler::FlashLineMachineList()
{

}


