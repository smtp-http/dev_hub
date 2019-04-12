#include <string>
#include <vector> 

using namespace std;

typedef struct StationEventProfile
{
	string Name;
	bool Enable;
	long long UpdateTimeTicks;
	string Action;
	string Flag;
	int PlcBlockAddress;        // port
	unsigned int PlcBlockSize;
	int EapBlockAddress;        // port
	unsigned int EapBlockSize;
} StationEventProfile_t;


vector<StationEventProfile_t*> *GetAllStationEventProfile(string xmlFile);