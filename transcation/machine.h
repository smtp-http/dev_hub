#ifndef __MACHINE_H_
#define __MACHINE_H_

#include <map>
#include <string>
#include "station.h"
#include "ev_respond.h"


class Machine 
{
public:
	Machine();
	~Machine();

	std::string GetName(){return m_name;}
	void SetName(std::string name){m_name = name;}
protected:
	std::map<std::string,StationEventProfile_t*> m_mainEvents;
	std::map<std::string,StationEventProfile_t*> m_stationsEvents;
	std::map<std::string,LineStation_t*> m_stations;

	void SetEvUpdater(IEventUpdater* eu){m_evUpdater = eu;}
	
private:
	std::string m_name;
	IEventUpdater* m_evUpdater;
};


class PlcMachine : public Machine
{
public:
	PlcMachine();
	~PlcMachine();
};


class Builder
{
public:
	virtual void BuildMainDeviceProfile(MainDeviceProfile_t *) = 0;
	virtual void BuildMainEvents(vector<StationEventProfile_t*>*) = 0;
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

	virtual void BuildMainDeviceProfile(MainDeviceProfile_t *);
	virtual void BuildMainEvents(vector<StationEventProfile_t*>*);

	virtual Machine* GetMachine(){return m_machine;}
	virtual void SetMachine(Machine *m){m_machine = m;}
};


class Director
{
public:
	Director();
	~Director();

	void ConstructMachine(LineMachine_t*);
protected:
	Builder  *m_builder;
	std::map<std::string,Machine*> m_machines;
};

class PlcDirector : public Director
{
public:
	PlcDirector(Builder *b){m_builder = b;}
	~PlcDirector();
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