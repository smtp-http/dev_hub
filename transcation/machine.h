#ifndef __MACHINE_H_
#define __MACHINE_H_

#include <map>
#include <string>
#include "station.h"
#include "ev_respond.h"
#include "PlcProxy.h"


#define OK 0
#define NO_SECTIONNAME_OR_MACINENAME -1


///////////////////////////////// Machine /////////////////////////////////////////

class Machine 
{
public:
	Machine();
	virtual ~Machine();

	std::string GetName(){return m_name;}
	void SetName(std::string name){m_name = name;}

	void EventPolling();

	Event* GetEvent(std::string evName);

	void PushMainEvent(std::string,Event*);
	void PushStationsEvent(std::string,Event*);

	void PushLineStation(std::string name,LineStation_t* ls)
	{
		//m_stations[name,ls];
		if (!m_stations.count(name))
			m_stations.insert (make_pair (name,ls));
	}

	void SetMachineContex(MachineContex* mc){m_contex = mc;}
	MachineContex* GetMachineContex(){return m_contex;}

protected:
	std::map<std::string,Event*> m_mainEvents;
	std::map<std::string,Event*> m_stationsEvents;
	std::map<std::string,LineStation_t*> m_stations;

	void SetEvUpdater(IEventUpdater* eu){m_evUpdater = eu;}

	//Protocol *m_protocol;
	MachineContex* m_contex;
	
private:
	std::string m_name;
	IEventUpdater* m_evUpdater;
};


class PlcMachine : public Machine
{
public:
	PlcMachine();
	virtual ~PlcMachine();
};


class Builder
{
public:
	virtual void BuildMachine(std::string sectionName,LineMachine_t*) = 0;
	virtual void BuildMainDeviceProfile(MainDeviceProfile_t *) = 0;
	virtual void BuildCustomEvents(vector<StationEventProfile_t*>*) = 0;
	virtual Machine* GetMachine()=0;
	virtual void SetMachine(Machine *) = 0;
protected:
	Machine* m_machine;
};

class MachinePlcBuilder : public Builder
{
public:
	MachinePlcBuilder(){m_machine=NULL;}
	~MachinePlcBuilder();

	virtual void BuildMachine(std::string sectionName,LineMachine_t*);
	virtual void BuildMainDeviceProfile(MainDeviceProfile_t *);
	virtual void BuildCustomEvents(vector<StationEventProfile_t*>*);

	virtual Machine* GetMachine(){return m_machine;}
	virtual void SetMachine(Machine *m){m_machine = m;}

};


class Director
{
public:
	Director();
	virtual ~Director();

	void ConstructMachine(std::string,LineMachine_t*);

	void MachinesPolling();

	int WriteMachineData(std::string sectionName,std::string machineName,std::string eventName,char *data);

protected:
	Builder  *m_builder;
	std::map<std::string,Machine*> m_machines;    // key = sectionName + machineName
};

class PlcDirector : public Director
{
public:
	PlcDirector(Builder *b){m_builder = b;}
	virtual ~PlcDirector(){}
};



class MachineScheduler 
{
public:
	~MachineScheduler();
	static MachineScheduler& GetInstance(std::string xmlFile);

private:
	MachineScheduler(std::string);

	void FlashLineMachineList();

	Director *m_plcDirector;

	std::map<std::string,LineSection_t*> m_sections;
};




#endif