#ifndef __STATION_H__
#define __STATION_H__

#include <string>
#include <vector> 

using namespace std;


typedef struct {
	std::string portName;
	int baud;
	char parity;
	int data_bit;
	int stop_bit;
}SerialParameter_t;

typedef struct StationEventProfile
{
	string Name;
	bool Enable;
	unsigned long long UpdateTimeTicks;
	string Action;
	string Flag;
	string Properties;
	string PlcBlockAddress;        // 
	unsigned int PlcBlockSize;
	string EapBlockAddress;        // 
	unsigned int EapBlockSize;
} StationEventProfile_t;

typedef struct ProtocolInfo
{
	string Protocol;
	int stationId;
}ProtocolInfo_t;

typedef struct Settings
{
	string Flag;
	string BeginAddress;
	unsigned int DataSize;
}Settings_t;

typedef struct PlcAlarmInfo
{
	string Name;
	string Remark;
	bool Enable;
	unsigned long long UpdateTimeTicks;
	bool Status;
	string Level;
	unsigned int Duration;
	unsigned int WordOffset;
	unsigned int BitOffset;
}PlcAlarmInfo_t;


typedef struct MainDeviceProfile
{
	string Name;
	bool Enable;
	unsigned long long UpdateTimeTicks;
	string DeviceType;
	ProtocolInfo_t protocolInfo;
	SerialParameter_t serialPara;
	string Priority;
	unsigned long ProcessTimeout;
	unsigned int Port;
	unsigned int WriteBufferSize;
	unsigned int WriteTimeout;
	unsigned int WriteRetryCount;
	unsigned long ReadTimeout;
	unsigned int ReadBufferSize;
	unsigned int KeepAliveInterval;
	string SockType;
	bool IsQueueMode;
	unsigned int RoutePort;
	unsigned int AutoReconnectInterval;
	string IpAddress;
	unsigned int PollFrequency;
	unsigned int HeartBeatFrequency;
	Settings_t NormalSettings;
	Settings_t MotorSettings;
	Settings_t PlcDataBlock;
	Settings_t EapDataBlock;
	vector<StationEventProfile_t*> mainEvents;
	vector<PlcAlarmInfo_t*> AlarmCodeList;

	string DataFormat;
}MainDeviceProfile_t;


typedef struct LineStation
{
	string Name;
	bool Enable;
	unsigned long long UpdateTimeTicks;
	unsigned int MaxInputCacheCarrierCount;
	unsigned int MaxOutputCacheCarrierCount;
	string Properties;
	vector<StationEventProfile_t*> Events;   // custom events
}LineStation_t;;


typedef struct LineMachine
{
	string Name;
	bool   Enable;
	unsigned long long UpdateTimeTicks;
	MainDeviceProfile_t mainDeviceProfile;

	vector<StationEventProfile_t*> *p_mainEvents;


	unsigned int CarrierSlotCount;
	unsigned int TraySlotCount;
	unsigned int Uph;
	vector<LineStation_t*> Stations;

	//vector<StationEventProfile_t*> *p_customEvents;
}LineMachine_t;


typedef struct LineSection
{
	string Name;
	bool Enable;
	unsigned long long UpdateTimeTicks;
	string SectionID;
	bool ReadOnly;
	vector<LineMachine_t*> *Machines;
}LineSection_t;


typedef struct LineStationDesiginProfile
{
	string Assembly;
	string PluginName;
	string Version;
	bool Enable;
	bool AutoLoaded;
	vector<LineSection_t*> *LineSections;
	unsigned int DefaultCarrierSlotCount;
	unsigned int DefaultTraySlotCount;

}LineStationDesiginProfile_t;


//vector<LineMachine_t*> *GetLineMachineList(string xmlFile);

//

#endif