#include "DeviceMonitor.h"
#include "Device.h"
#include "Scanner.h"
#include "Usb.h"

using namespace lux;
using namespace std;

std::vector<Device> get_keyboards(const String& p="/proc/bus/input/devices");
extern lux::IUpdateSink *g_updater;

DeviceMonitor& DeviceMonitor::GetInstance()
{
	static  DeviceMonitor instance_;
	return instance_; 

}

DeviceMonitor::DeviceMonitor()
{
	struct timeval tv={1, 0};
	m_devCheck = EventLooper::GetInstance().ScheduleTimer(&tv, TF_FIRE_PERIODICALLY, this);
	

}

DeviceMonitor::~DeviceMonitor()
{

}



void DeviceMonitor::OnTimer(TimerID tid)
{
	int flag = 0;
	string dev;
	map<string,IPeripheral*>::iterator l_it;
	//auto keyboards = get_keyboards();
	vector<Device> keyboards = get_keyboards();
	IEquipment *equ;
//	for(const auto& k : keyboards){
//		std::cout<<"\t"<<k.get_handler_starting_with("event")<<"\n";
//	}

	for (map<string, IPeripheral *>::iterator i = m_peripherals.begin(); i != m_peripherals.end(); i++) {  
		//cout << "dev: " << i->first << endl;
		for (vector<Device>::const_iterator iter = keyboards.cbegin(); iter != keyboards.cend(); iter++) {
			//cout << (iter->get_handler_starting_with("event")) << endl;
			
			if(iter->get_handler_starting_with("event") == i->first) {
				//cout << "find dev: " << i->first << endl;
				//goto _end;
				flag = 1;
				break;
			}
		}

		if(!flag) {
			cout << "dev: " << i->first << " is leave!\n";
			i->second->Close();
			delete i->second;
			m_peripherals.erase(i);  
			// TODO: Remove from transcation
		} else {
			flag = 0;
		}

	}  

	l_it = m_peripherals.begin();

	for(const auto& k : keyboards) {
		//std::cout<<"\t"<<k.get_handler_starting_with("event")<<"\n";
		string dev = k.get_handler_starting_with("event");
		//cout << dev << endl;

		l_it=m_peripherals.find(dev);
		if(l_it==m_peripherals.end()) {
			Usb* usb = new Usb(dev);
			usb->OpenPeripheral();
			if(g_updater != NULL) {
				equ = new HandhelScanner(g_updater,usb->GetDevName());
				usb->SetEquipment(equ);
			}
			m_peripherals[dev] = usb;
			// TODO: add to transcation
			cout << "new usb scanner is ready!  dev:" << dev << endl;
		}
	}

	
	
}




