#include <iostream>
#include <fcntl.h>  
#include <linux/input.h>
#include <unistd.h>
#include <string.h>
#include "Scanner.h"
#include "Usb.h"

using namespace std;
using namespace lux;

void on_read(int fd, short event, void* arg);

Usb::Usb(string devName)
{
	m_devName = "/dev/input/" + devName;
	memset(m_readBuffer,0,1024);
	m_writeBusy = 0;
}

Usb::~Usb()
{
	
}

int Usb::OpenPeripheral()
{
	//m_fd = open(m_devName.c_str(),O_RDONLY);
	int fd;
	fd = open(m_devName.c_str(),O_RDWR);
	if (fd < 0){ 
		perror("can not open device usbscanner!"); 
		return -1;//exit(1); 
	}

	m_fd = fd;
	cout << "open dev: " << m_devName << " fd:" << m_fd << endl;


	struct event *usb_ev = event_new(m_evBase,m_fd, EV_READ|EV_PERSIST,on_read,this);  

	event_add(usb_ev, NULL);

	m_rwEvent = usb_ev;
	
	return 0;
}




/*

void Usb::SetUpdater(IUpdateSink *updater)
{
	m_equipment = new HandhelScanner(updater,m_devName);
}



void Usb::OnRead(int fd)
{
	static int readBufLen = 0;
	//int nread;
	struct input_event inv; 
	while(read(fd,&inv,sizeof(struct input_event)) > 0){
		if(inv.type == 1) {
			printf("%d ",inv.code);
			if(inv.code == 28){
				printf("++++ on bar code!\n");
				if(m_equipment != NULL){
					m_equipment->OnData(m_readBuffer, readBufLen);
					readBufLen = 0;
				}
				//continue;
			} else {
				m_readBuffer[readBufLen ++] = inv.code;
			}
		}
	}
}
*/

//void Usb::Close(int fd)
//{
//	event_del(m_rwEvent);
//}
