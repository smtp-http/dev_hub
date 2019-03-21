#include <iostream>
#include <vector>
#include <chrono>
//using namespace std;
#include "ThreadPool.h"
////////////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <list>
#include <iostream>
#include <algorithm>
#include "json/json.h"
#include "connbase.h"
#include "mqtt.h"
//#include "eventlooper.h"
#include "eventlooperimp.h"
#include "TcpServer.h"
#include "UnixSockServer.h"
#include <time.h>
#include "DeviceMonitor.h"
#include "Transcation.h"
#include "SolderCtrl.h"
#include "ElecBalance.h"
#include "FunctionManager.h"
//#include "Log4cxxWrapper.h"



extern "C"
{
#include "Http.h"
#include "ut_title.h"
#include "ut_misc.h"
#include "ut_log.h"

}

using namespace std;
using namespace lux;


lux::IUpdateSink *g_updater;


const char *__process__ = "dev_collecter";
const char *__version__ = "0.1.0";



string task1(int i){
	//for(;;){
		std::cout << "scanner ... " << i * 10 << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
//		el=el;
		return "hello world!\n";
	//}
}

Equipment *BuildElecBalance(int equipNum,IUpdateSink *updater)
{
	return new ElecBalance(equipNum,updater);
}


Equipment *BuildSolderCtrl(int equipNum,IUpdateSink *updater)
{
	return new SolderCtrl(equipNum,updater);
}

CommServer *BuildTcpServer(const string ip,short port)
{
	TcpServer *server = new TcpServer(ip, port);//"10.10.80.15"
	if (!server->Start()){
			cerr << "server start failed." << endl;
			return NULL;
	}

	return server;
}

CommServer *BuildUnixSocketServer(const string unixSock)
{
	return new UnixSockServer(unixSock);
}




void UartTest(lux::IUpdateSink *updater);
//TcpServer *TcpServerTest();
TcpServer *TcpServerTest();
UnixSockServer *UnixSockServerTest();

#include <unistd.h>

void* open_uart_thread(void* arg){
   

	usleep(8000000);
	UartTest((lux::IUpdateSink *)arg);

	return 0;
}

int DeviceTest();
int start_scanner(string dev);

/*
void TranscationTest()
{
	TranscationManager &mng = TranscationManager::GetInstance();
//	IUpdateSink *server
	TcpServer *tcpSvr = TcpServerTest();
	if (tcpSvr == NULL) {
		return;
	}
	tcpSvr->SetDownChannel(&mng);
	
	Transcation *hs = new TranscationHandheldScanner(1);
	hs->AddServer(tcpSvr);
	hs->Build();

	mng.AddTranscation(hs);
}
*/



//void transcationTest();
void transcationTest()
{
	TranscationManager &manager = TranscationManager::GetInstance();
	SysConfig::Instance().AddUser(&manager);

	SysConfig::Instance().LoadUserConfig(manager);
	
}



int main(int argc, char **argv)
{
	//TranscationManager &tm = TranscationManager::GetInstance();
	FunctionManager &fm = FunctionManager::GetInstance();

	SysConfig &sc = SysConfig::Instance();

	//Mqtt::mqtt();
	//HttpPoster::UpdateInfo();
	
	printf("process: %s version: %s, compile date: %s %s\n", __process__, __version__, __DATE__, __TIME__);
	//sc.LoadUserConfig(tm);
	sc.LoadUserConfig(fm);

	
#ifdef __kepalive
	if (process_exist(__process__) != 0) {
        printf("process: %s exist\n", __process__);
        exit(EXIT_FAILURE);
    }

    process_title_init(argc, argv);

	daemon(1, 1);
	process_keepalive();
#endif

	EventLooperImp &el = (EventLooperImp&)EventLooper::GetInstance();
	cout << "-- event base: " << el.CurrentEvBase() << endl;
	el.RunEventLoop();

	cout << "======***====" << endl;

    return 0;
}


/*
void fins_test();

int main()
{
	fins_test();
	return 0;
}
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

