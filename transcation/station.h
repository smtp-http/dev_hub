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



vector<StationEventProfile_t*> *GetAllStationEventProfile(string xmlFile);