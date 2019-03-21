#include <iostream>
#include "Sequence.h"
#include "SolderCtrl.h"
#include <iterator>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string.h>



using namespace lux;
using namespace std;


void split(const string &str, char delim, vector<string> &elems, bool skip_empty = true) 
{
    istringstream iss(str);
    for (string item; getline(iss, item, delim); )
        if (skip_empty && item.empty()) continue;
        else elems.push_back(item);
    return; //elems;
}


std::string Convert (float number){
    std::ostringstream buff;
    buff<<number;
    return buff.str();   
}



/////////////////////////////// SequenceState //////////////////////////
void SequenceState::ChangeState(SequenceState * s)
{
	m_sequenceEquipmen->ChangeState(s);
}


//////////////////////////// SequenceIdleState /////////////////////////

int SequenceIdleState::RecvDownStream(const string msg)
{
	cout << "*** idel state " << endl;
	//if (m_type == GlueSequence) {
	if ( msg == "GetSpitParam") {
		char buf[] = {_ENQ};

		m_sequenceEquipmen->WritePeripheralData(buf,1);
		cout << "change state upload" << endl;
		//if ( m_sequenceEquipmen->m_type == GlueSequence)
		ChangeState(&SequenceUpLoadState::Instance(m_sequenceEquipmen->m_type,m_sequenceEquipmen));
		//else if(m_sequenceEquipmen->m_type == SolderSequence) 
			//ChangeState(&SequenceUpLoadRepA0State::Instance(m_sequenceEquipmen->m_type,m_sequenceEquipmen));
	}
	//} else if (m_type == SolderSequence) {

	//}
}

int SequenceIdleState::RecvUpstream(const char * data, int len)
{

}


SequenceIdleState &SequenceIdleState::Instance(SequenceType type,SolderCtrl *equip)
{
	static SequenceIdleState _state;
	//_state.m_type = type;
	_state.m_sequenceEquipmen = equip;
	return _state;
}



///////////////////////////// SequenceUpLoadState //////////////////////
SequenceUpLoadState& SequenceUpLoadState::Instance(SequenceType type,SolderCtrl *equip)
{
	static SequenceUpLoadState _state;
	//_state.m_type = type;
	_state.m_sequenceEquipmen = equip;
	return _state;
}

int SequenceUpLoadState::RecvDownStream(const string msg)
{
	cout << "*** upload state " << endl;
	
}

int SequenceUpLoadState::RecvUpstream(const char * data, int len)
{
	
	if (data[0] == _ACK) {
		cout << "upload ==========" << endl;
		if ( m_sequenceEquipmen->m_type == GlueSequence){
			
			char buf[]={_STX,'0','8','U','L','0','0','0',',','0','1','D','A',_ETX};
			cout << "$ write glue data" << endl;
			m_sequenceEquipmen->WritePeripheralData(buf,sizeof(buf));
			
			ChangeState(&SequenceUpLoadRepState::Instance(m_sequenceEquipmen->m_type,m_sequenceEquipmen));
		} else if(m_sequenceEquipmen->m_type == SolderSequence) {
			
			char buf[] = {_STX,'0','8','U','L','0','0','1','D','0','1','C','1',_ETX};
			cout << "$ write solder data" << endl;
			m_sequenceEquipmen->WritePeripheralData(buf,sizeof(buf));
			
			ChangeState(&SequenceUpLoadRepA0State::Instance(m_sequenceEquipmen->m_type,m_sequenceEquipmen));
		}
	}
}


///////////////////////////// SequenceUpLoadRepState /////////////////////
SequenceUpLoadRepState& SequenceUpLoadRepState::Instance(SequenceType type, SolderCtrl * equip)
{
	static SequenceUpLoadRepState _state;
	//_state.m_type = type;
	_state.m_sequenceEquipmen = equip;
	return _state;

}

int SequenceUpLoadRepState::RecvDownStream(const string msg)
{

	return 0;
}

string intToString(int v)  
{  
    char buf[32] = {0};  
    snprintf(buf, sizeof(buf), "%u", v);  
  
    string str = buf;  
    return str;  
}


int SequenceUpLoadRepState::RecvUpstream(const char * data, int len)
{
	string resp;
	
	if ( m_sequenceEquipmen->m_type == GlueSequence){
		vector<string> elems;
		string result = data;
		split(result, ',', elems);
		float time,pressure;
		
		for (int i = 0;i < elems.size();i ++) {
			//cout << "-- " << i << " -- " << elems[i] << endl;
			if (i == 3) {
				const char *dt = elems[i].c_str();
				float data = (dt[0] - 0x30) * 100000 + (dt[1] - 0x30) * 10000 + (dt[2] - 0x30) * 1000 + (dt[3] - 0x30) * 100 + (dt[4] - 0x30) * 10 + dt[5] - 0x30;
				time = data/100;
			}

			if ( i == 11) {
				const char *dt = elems[i].c_str();
				float data = (dt[0] - 0x30) * 10000 + (dt[1] - 0x30) * 1000 + (dt[2] - 0x30) * 100 + (dt[3] - 0x30) * 10 + dt[4] - 0x30;
				pressure = data/100;
			}
		}

		resp = "{\"Press\":" + Convert(pressure) + ",\"time\":" + Convert(time) + "}";

	} else if(m_sequenceEquipmen->m_type == SolderSequence) {
		int pressure = (data[8] - 0x30)*1000 + (data[9] - 0x30)*100 + (data[10] - 0x30)*10 + (data[11] - 0x30);
		int time = (data[13] - 0x30)*10000 + (data[14] - 0x30)*1000 + (data[15] - 0x30)*100 + (data[16] - 0x30)*10 + (data[17] - 0x30);

		resp = "{\"Press\":" + intToString(pressure) + ",\"time\":" + intToString(time) + "}";
	}

	
		//"{\"cmd\":\"GetSpitParam\", \"equipment\":\" GlueDispenser \",\"equipNum\":0,	\"resultCode\":0,\"result\":{\"Pressure\":" + intToString(pressure) +"}";
	m_sequenceEquipmen->UpStream(resp.c_str(),resp.length());

	char buf[] = {_EOT};
	m_sequenceEquipmen->WritePeripheralData(buf,sizeof(buf));
		
	ChangeState(&SequenceIdleState::Instance(m_sequenceEquipmen->m_type,m_sequenceEquipmen));

	return 0;
}


/////////////////////////// SequenceUpLoadRepA0State //////////////////////

SequenceUpLoadRepA0State& SequenceUpLoadRepA0State::Instance(SequenceType type, SolderCtrl * equip)
{
	static SequenceUpLoadRepA0State _state;
	//_state.m_sequenceEquipmen->m_type = type;
	_state.m_sequenceEquipmen = equip;
	return _state;
}

int SequenceUpLoadRepA0State::RecvDownStream(const string msg)
{

	return 0;
}

int SequenceUpLoadRepA0State::RecvUpstream(const char * data, int len)
{
	cout << "2.1 ------------------ upload rep A0 state recv upstream ----------------" << endl;

	
	if(data[3] == 'A' && data[4] == '2') {
		cout << "recv A2 resp,err" << endl;
		char buf[] = {_STX,'0','2',_CAN,_CAN,'6','E',_ETX};
		
		m_sequenceEquipmen->WritePeripheralData(buf,sizeof(buf));

		
		char buf2[] = {_EOT};
		m_sequenceEquipmen->WritePeripheralData(buf2,1);
		
		
		string resp = "{\"result\":null}";
		m_sequenceEquipmen->UpStream(resp.c_str(),resp.length());
		
		ChangeState(&SequenceIdleState::Instance(m_sequenceEquipmen->m_type,m_sequenceEquipmen));

		return 0;
	}
	
	char buf[] = {_ACK};
	m_sequenceEquipmen->WritePeripheralData(buf,1);
	
	ChangeState(&SequenceUpLoadRepState::Instance(m_sequenceEquipmen->m_type,m_sequenceEquipmen));
	
	return 0;
}



///////////////////////////// SequenceDownLoadState ///////////////////////
SequenceDownLoadState& SequenceDownLoadState::Instance(SequenceType type,SolderCtrl *equip)
{
	static SequenceDownLoadState _state;
	//_state.m_sequenceEquipmen->m_type = type;
	_state.m_sequenceEquipmen = equip;
	return _state;
}

int SequenceDownLoadState::RecvDownStream(const string msg)
{
	cout << "*** download state " << endl;

}

int SequenceDownLoadState::RecvUpstream(const char * data, int len)
{

}



