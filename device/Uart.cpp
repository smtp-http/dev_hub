#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>  
#include <fcntl.h>  
#include <termios.h>
#include <errno.h> 
#include <pthread.h> 
#include <sys/ioctl.h> 
#include <iostream>
#include "CodePairing.h"
#include "Uart.h"
#include "Scanner.h"


using namespace std;

using namespace lux;

#define FALSE 1
#define TRUE 0



int speed_arr[] = {  B115200, B57600, B38400, B19200, B9600, B4800,B2400, B1200};
int name_arr[] = {115200, 57600, 38400,  19200,  9600,  4800,  2400, 1200};

//////////////////////////// function /////////////////////////////
void on_read(int fd, short event, void* arg);



//////////////////////////////// public //////////////////////////

Uart::Uart(string devName)
	: m_baudRate(115200)
{
	m_devName = devName;
	m_equipment = NULL;
	m_writeBusy = 0;
}


Uart::Uart(std::string devName,int baudRate)
{
	int i,len;

	GET_ARRAY_LEN(name_arr,len);

	for (i = 0; i < len; i ++) {
		if (baudRate == name_arr[i]) {
			m_baudRate = baudRate;
			break;
		}
	}
	
	m_devName = devName;
	m_equipment = NULL;
	m_writeBusy = 0;
	
}



Uart::~Uart()
{

}

void Uart::SetBaudRate(int baudRate)
{
	int i,len;

	GET_ARRAY_LEN(name_arr,len);

	for (i = 0; i < len; i ++) {
		if (baudRate == name_arr[i]) {
			m_baudRate = baudRate;
			break;
		}
	}
}


int Uart::OpenPeripheral()
{
	int fd;
    fd = open(m_devName.c_str(), O_RDWR);
	if (fd < 0){
		printf("open device %s faild\n", m_devName.c_str());
		return -1;
	}

	m_fd = fd;

    cout<<"===== fd:%d  "<< fd << endl;
	SetSpeed(m_fd,m_baudRate); //
	
	SetParity(m_fd,8,1,'N'); //

	struct event *uart1_ev = event_new(m_evBase,m_fd, EV_READ|EV_PERSIST,on_read,this);  

	event_add(uart1_ev, NULL);

	m_rwEvent = uart1_ev;

	//cout << "++ event base: " << m_evBase << endl;

	return 0;
}


///////////////////////////////////////// private /////////////////////////////////////

void Uart::SetSpeed(int fd, int speed)
{
    int i;
    int status;
    struct termios Opt;
    tcgetattr(fd, &Opt);
    for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++)
    {
	   	if (speed == name_arr[i])
	   	{
	   	    tcflush(fd, TCIOFLUSH);
		    cfsetispeed(&Opt, speed_arr[i]);
		    cfsetospeed(&Opt, speed_arr[i]);
		    status = tcsetattr(fd, TCSANOW, &Opt);
		    if (status != 0)
				perror("tcsetattr fd1");
			return;
     	}
		tcflush(fd,TCIOFLUSH);
    }
}
int Uart::SetParity(int fd,int databits,int stopbits,int parity)
{
    struct termios options;
	
    if ( tcgetattr( fd,&options)  !=  0)
    {
	  	perror("SetupSerial 1");
	  	return(FALSE);
    }
	
    options.c_cflag &= ~CSIZE;
    switch (databits)
    {
	    case 7:
			options.c_cflag |= CS7;
			break;
	    case 8:
			options.c_cflag |= CS8;
			break;
	    default:
			fprintf(stderr,"Unsupported data size\n");
			return (FALSE);
    }
    switch (parity)
    {
	    case 'n':
	    case 'N':
			options.c_cflag &= ~PARENB;   
			options.c_iflag &= ~INPCK;   
			break;
	    case 'o':
	    case 'O':
			options.c_cflag |= (PARODD | PARENB); 
			options.c_iflag |= INPCK;           
			break;
	    case 'e':
	    case 'E':
			options.c_cflag |= PARENB;     
			options.c_cflag &= ~PARODD;
			options.c_iflag |= INPCK;     
			break;
	    case 'S':
	    case 's':  
			options.c_cflag &= ~PARENB;
			options.c_cflag &= ~CSTOPB;
			break;
	    default:
			fprintf(stderr,"Unsupported parity\n");
			return (FALSE);
    }
    switch (stopbits)
    {
	    case 1:
			options.c_cflag &= ~CSTOPB;
			break;
	    case 2:
			options.c_cflag |= CSTOPB;
			break;
	    default:
			fprintf(stderr,"Unsupported stop bits\n");
			return (FALSE);
    }


	options.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
	options.c_oflag &= ~OPOST;
	options.c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);

    /* Set input parity option */
    
    if (parity != 'n') {
		options.c_iflag |= INPCK;
	    options.c_cc[VTIME] = 150; // 15 seconds
	    options.c_cc[VMIN] = 0;
    }
    
	tcflush(fd,TCIFLUSH); /* Update the options and do it NOW */
    if (tcsetattr(fd,TCSANOW,&options) != 0)
    {
		perror("SetupSerial 3");
		return (FALSE);
    }
    return (TRUE);
}





////////////////////////////////////// test ////////////////////////////////////////
void creat_write_thread(int fd);

enum ConnectionDevType {ConScanner,ConSolderWeigher,ConSolderController};

void UartTest(lux::IUpdateSink *updater)
{
	ConnectionDevType type;

	type = ConScanner;

	if (type == ConScanner) {

///////////// Connection scanner ///////////////////

		cout << "open uart 1:" << endl;
		Peripheral *per1 = new Uart("/dev/ttyO1");
		per1->OpenPeripheral();

		CodePairing *cp = &CodePairing::GetInstance();
		cp->SetTcpTransfer(updater);

		per1->SetEquipment(new KeyenceScanner(cp,per1->GetDevName()));
		
		creat_write_thread(per1->GetFd());

		Peripheral *per2 = new Uart("/dev/ttyO2");
		per2->OpenPeripheral();
		per2->SetEquipment(new KeyenceScanner(cp,per2->GetDevName()));

		creat_write_thread(per2->GetFd());

	} else if(type == ConSolderWeigher) {
		
	} else if (type == ConSolderController) {

	}
	
}


void* write_thread(void* arg){
	char data[20];

    long long_fd = (long)arg;
    int fd = (int)long_fd;
	cout<<"write_thread() start! fd:" << fd <<endl;
    if(fd<0){
        cout<<"process_in_new_thread_when_accepted() quit!"<<endl;
        return 0;
    }
	
	 while(1) {
    //cout << "wh----";
      //printf("Please Input string to send:\n:");
	  		memset(data,0,20);
			sprintf(data,"-- %d --  ",fd);
			//printf("%d\n",data);
			write(fd, data, strlen(data));  
			//write(fd, "\n", strlen("\n"));
				usleep(2000000);	 
				//cout<<"write() fd:  "<< fd << endl;
      	
 	}      
    close(fd);
	 
    return 0;
}


void creat_write_thread(int fd) {
    pthread_t thread;
    pthread_create(&thread,NULL,write_thread,(void*)fd);
    pthread_detach(thread);
}


