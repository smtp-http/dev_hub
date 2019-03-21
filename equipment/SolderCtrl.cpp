#include <iostream>
#include <string.h>
#include "SolderCtrl.h"

using namespace lux;
using namespace std;


Equipment *BuildGlueDispenser(int equipNum,IUpdateSink *updater)
{
	return new GlueDispenser(equipNum,updater);
}





char checksum(char *data,int len)
{
	int i;
	char cs = 0;

	for (i = 0;i < len;i ++) {
		cs = cs - *(data + i);
	}

	return cs;
}

//////////////////////////////////// SolderCtrl ////////////////////////////

int SolderCtrl::OnData(char *data, int len)
{
	if ( data[0] == _ACK) {
		m_state->RecvUpstream(data,len);
		return 0;
	}

	for ( int i = 0;i < len;i ++) {
		if(data[i] != _ETX) {
			m_equipData.push_back(data[i]);
		} else {
			char buf[100];
			memset(buf,0,100);

			int count = m_equipData.size();
			for (int i = 0;i < count;i ++) {
				buf[i] = m_equipData[i];
			}
			
			m_state->RecvUpstream(buf,count);
			m_equipData.clear();
		}
	}
	
	//m_state->RecvUpstream(data,len);
	
}


void SolderCtrl::OnTaskTimeout()
{
	cout << "@@@@ solder ctrl on task time out!" << endl;
	m_state = &SequenceIdleState::Instance(SolderSequence,this);
}

SolderCtrl::SolderCtrl(int devNum,IUpdateSink *updater)
{
	m_devNumber = devNum;
	if (updater != NULL)
		m_updater = updater;

	m_type = SolderSequence;

	m_state = &SequenceIdleState::Instance(SolderSequence,this);
}

void SolderCtrl::UpStream(const char *data, int len)
{
	m_updater->UpdateInfo(data,len);
}


int SolderCtrl::RecvCommand(const string cmd)
{
	cout << "*** recv solder command" << endl;
	m_state->RecvDownStream(cmd);
}


void SolderCtrl::ChangeState(SequenceState * s)
{
	if (s != NULL)
			m_state = s;
}

int SolderCtrl::WritePeripheralData(const char * data, int len)
{
	m_peripheral->WriteData(data,len);
	return 0;
}


//int SolderCtrl::Submit(SequenceType type, unsigned char * data, int len)
//{

//}

///////////////////////////////////////  GlueDispenser //////////////////////////////////

GlueDispenser::GlueDispenser(int equipNum, IUpdateSink * updater)
{
	m_devNumber = equipNum;
	if (updater != NULL) {
		m_updater = updater;
	}

	m_type = GlueSequence;
	
	m_state = &SequenceIdleState::Instance(GlueSequence,this);
}


//int GlueDispenser::OnData(char *data, int len)
//{

//}


int GlueDispenser::RecvCommand(const string cmd)
{
	cout << "*** recv glue command: " << cmd << endl;
	m_state->RecvDownStream(cmd);
}


//int GlueDispenser::Submit(SequenceType type, unsigned char * data, int len)
//{

//}


