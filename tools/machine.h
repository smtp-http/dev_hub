#ifndef __MACHINE_H_
#define __MACHINE_H_

#include <map>
#include <string>
#include "station.h"


class Machine 
{
public:
	Machine();
	~Machine();

protected:
	std::map<std::string,StationEventProfile_t*> m_mainEvents;
	std::map<std::string,StationEventProfile_t*> m_stationsEvents;
	LineMachine_t *m_machine;
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

protected:
	Machine* m_machine;
};

class MachinePlcBuilder : public Builder
{
public:
	MachinePlcBuilder(){m_machine=NULL;}
};


class Director
{
public:
	Director();
	~Director();

protected:
	Builder  *m_builder;
	std::map<std::string,Machine*> m_machine;
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
	MachineScheduler(std::string);
	~MachineScheduler();
	static MachineScheduler& GetInstance(std::string xmlFile);

private:
	void FlashLineMachineList();

	Director *m_plcDirector;
	std::string m_xmlFile;
};




#endif