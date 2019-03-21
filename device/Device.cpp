#include "Device.h"


#define IDEV_KEYBOARD_EVENTS "120013"
#define IDEV_MOUSE_EVENTS "17"
#define IDEV_B_EVENTS "EV"

using namespace lux;


//filters device list, searching for devices where the "B" attribute attrib
//matches value
std::vector<Device> filter_B(const std::vector<Device>& in,
							 const String& attrib,
							 const String& value)
{
	std::vector<Device> out;
	for(const auto& d : in) {
		auto it = d.B.find(attrib);
		if(it != d.B.end() && (*it).second == value)
			out.push_back(d);
	}
	return out;
}

// reads all devices from proc/bus/input/devices
std::vector<Device> get_devices(const String& p="/proc/bus/input/devices")
{
	std::ifstream f;
	f.open(p);
	if(!f.is_open())
		std::cout<<"cannot open /proc/bus/input/devices"<<std::endl;
	std::vector<Device> res;
	Device dev;
	dev.B.clear();
	dev.H.clear();
	while(dev.read(f)) {
		res.push_back(dev);
		dev.B.clear();
		dev.H.clear();
	}
	
	f.close();
	return  res;
}

std::vector<Device> get_keyboards(const String& p="/proc/bus/input/devices")
{
	auto devs = get_devices(p);
	return  filter_B(devs,IDEV_B_EVENTS,IDEV_KEYBOARD_EVENTS);
}

std::vector<Device> get_mice(const String& p = "/proc/bus/input/devices")
{
	auto devs = get_devices(p);
	return  filter_B(devs,IDEV_B_EVENTS,IDEV_MOUSE_EVENTS);
}

//}


/////////////////////////////////// test ///////////////////////////////////


int DeviceTest()
{
	auto keyboards = get_keyboards();
	auto mice = get_mice();
	
	std::cout<<"keyboards:\n";
	for(const auto& k : keyboards)
	{
		std::cout<<"\t"<<k.get_handler_starting_with("event")<<"\n";
	}
	std::cout<<"mice:\n";
	for(const auto& m : mice)
	{
		std::cout<<"\t"<<m.get_handler_starting_with("event")<<"\n";
	}
	
	return 0;
}





