#include <iostream>
#include <unistd.h>
#include "Peripheral.h"
#include "eventlooperimp.h"

using namespace lux;
using namespace std;


void on_read(int fd, short event, void* arg)
{
	Peripheral *peripheral = (Peripheral*)arg;
	if (event & EV_READ){
		if (peripheral){
			peripheral->OnRead(fd);
		}
	}
}


Peripheral::Peripheral()
{
	cout << "get event base!" << endl;
	EventLooperImp &el = (EventLooperImp&)EventLooper::GetInstance();
	m_evBase = el.CurrentEvBase();
	m_equipment = NULL;
}

Peripheral::~Peripheral()
{
	if(m_equipment != NULL)
		delete m_equipment;
}

void Peripheral::Close()
{
	RemoveEquipment();
	event_del(m_rwEvent);
	close(m_fd);
	cout << "close dev: " << m_devName << " fd:" << m_fd << endl;
}



int Peripheral::GetFd()
{
	return m_fd;
}


void Peripheral::OnRead(int fd)
{
	int nread;
	if((nread = read(fd,m_readBuffer,1000))>0){
		
		if(m_equipment != NULL){
			
			m_equipment->OnData(m_readBuffer, nread);
		}
		
	}
}

int Peripheral::WriteData(string & data)
{
	cout << "write data: " << data << endl;
	
	write(m_fd, data.data(),data.length());
}


int Peripheral::WriteData(const char *data,int len)
{
	write(m_fd,data,len);
}


void Peripheral::SetEquipment(IEquipment *equ)
{
	if (equ != NULL) {
		m_equipment = equ;
	}
}


int  Peripheral::RecvCommand(const string cmd)
{
	return m_equipment->RecvCommand(cmd);
}

void Peripheral::RemoveEquipment()
{
	if(m_equipment != NULL) {
		delete m_equipment;
		m_equipment = NULL;
	}
}

void Peripheral::SetEquipmentName(const string name)
{
	m_equipmentName = name;
}

void Peripheral::SetEquipmentNo(const int no)
{
	m_equipmentNo = no;
}

string Peripheral::GetEquipmentName()
{
	return m_equipmentName;
}

int Peripheral::GetEquipmentNo()
{
	return m_equipmentNo;
}


void Peripheral::MarkTime()
{
	m_equipment->TimeMark();
}


