#ifndef __STATION_H__
#define __STATION_H__

#include <string>
#include <vector> 

using namespace std;



typedef struct StationEventProfile
{
	string Name;
	bool Enable;
	unsigned long long UpdateTimeTicks;
	string Action;
	string Flag;
	string Properties;
	int PlcBlockAddress;        // 
	unsigned int PlcBlockSize;
	int EapBlockAddress;        // 
	unsigned int EapBlockSize;
} StationEventProfile_t;

typedef struct ProtocolInfo
{
	string Protocol;
}ProtocolInfo_t;

typedef struct Settings
{
	string Flag;
	string BeginAddress;
	unsigned int DataSize;
}Settings_t;


typedef struct MainDeviceProfile
{
	string Name;
	bool Enable;
	unsigned long long UpdateTimeTicks;
	string DeviceType;
	ProtocolInfo_t protocolInfo;
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
	std::string Name;
}LineSection_t;


typedef struct LineStationDesiginProfile
{
	
}LineStationDesiginProfile_t;


vector<LineMachine_t*> *GetLineMachineList(string xmlFile);

LineStationDesiginProfile_t* GetLineStationDesiginProfile(std::string xmlFile);

#endif