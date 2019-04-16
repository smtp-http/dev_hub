#ifndef __MACHINE_H_
#define __MACHINE_H_

#include <map>
#include <string>

class Machine 
{
public:
	Machine();
	~Machine();
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
	MachineScheduler();
	~MachineScheduler();
	static MachineScheduler& GetInstance();

private:
	void FlashLineMachineList();

private:
	Director *m_plcDirector;
};




#endif