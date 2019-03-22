#include "PlcFins.h"
#include <string.h>
#include <iostream>
#include <sstream>



using namespace std;



void ftoc(float fvalue,unsigned char*arr) 
{ 
	unsigned char *pf; 
	unsigned char *px; 
	unsigned char i; //计数器 
	pf =(unsigned char *)&fvalue; /*unsigned char型指针取得浮点数的首地址*/ 
	px = arr; /*字符数组arr准备存储浮点数的四个字节,px指针指向字节数组arr*/ 
	for(i=0;i<4;i++) { 
		*(px+i)=*(pf+i); /*使用unsigned char型指针从低地址一个字节一个字节取出*/ 
	} 
} 


float ByteToFloat(unsigned char* byteArray) 
{ 
	return *((float*)byteArray); 
} 





void fins_test()
{
	int i;
	unsigned char byteArray[4];
    ftoc(-12.12345,byteArray);


	struct fins_sys_tp *sys;
	int ret;

	//sys.address = "192.168.1.1";
	//sys.port = 9600;
	int error_val;
	
	sys = finslib_tcp_connect(NULL, "172.17.208.152", 9600, 0, 10, 0, 0, 1,0,&error_val, 6);
	printf("0x%x   error_val: %d\n",sys,error_val);
	if(sys == NULL || error_val != 0) {
		printf("connect error!\n");
		return;
	}



	int a;
	float f1 = 12.2312;
	printf("f1: %f\n",f1);
	int *b = (int *)byteArray;
	printf("f1 : 0x%x   b: 0x%x\n",f1,b);

	sys->plc_mode = FINS_MODE_CS;
	//struct fins_cpudata_tp *cpudata = new(struct fins_cpudata_tp);

	//ret = finslib_cpu_unit_data_read(sys,cpudata);

	//printf("cpudata read ret:%d\n",ret);

	unsigned char data[] = {0x30,0x39,0x01,0x02};
	unsigned char rddata[10];
	memset(rddata,0,10);
	unsigned short rdwd = 0;

	char start[] = "DM15170";
 
	ret = finslib_memory_area_read_word(sys,start,(unsigned char *)(&rdwd)/*rddata*/,1);
	//ret = finslib_memory_area_read_int32( sys, start, &a, 1 );
	printf("+++ret:%d\n",ret);
	//short *rd = (*short)rddata;
	//printf("rddata[%d] = 0x%x\n",0,rddata[0]);
	//printf("rddata[%d] = 0x%x\n",1,rddata[1]);
	//printf("=== 0x%x\n",*((short *)rddata));
	printf("==== rd wd: 0x%x",rdwd);


	//for(int i = 0; i < 8; i ++ ) {
	//	printf("rddata[%d]: 0x%x \n",i,rddata[i]);
	//} 
	//printf("rddata[0]:  %d   rddata[1]: %d\n",rddata[0],rddata[1] );

	if(ret != FINS_RETVAL_SUCCESS)
		return;

	char data_start[10] = "DM15174";
	ret = finslib_memory_area_write_int32( sys, data_start,b, 1 ) ;
	//ret = finslib_memory_area_write_word(sys,start,byteArray,2);
	printf("------ret:%d\n",ret);


	if(ret != FINS_RETVAL_SUCCESS)return;

	printf("========== ");

	//unsigned char read_data[2];

	//ret = finslib_memory_area_read_word(sys,start, read_data, 2);

	//printf("ret:%d    read_data0:0x%x   read_data1:0x%x\n",ret,read_data[0],read_data[1]);

	finslib_disconnect(sys);
}



string& trim(string &s) 
{
	if (s.empty()) {
		return s;
	}

	s.erase(0,s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	return s;
}



/////////////////////////////////////////////  Plc  ///////////////////////////////////////////////////
Plc::Plc(string ip,int port)
	: m_ip(ip)
	, m_port(port)
{
	m_sys = NULL;
	m_connectionStatus = CONNECT_NO;
}


int Plc::Connect()
{
	int error_val;
	std::cout << "ip:" << m_ip << "  port: " << m_port << std::endl;

	if ( m_connectionStatus == CONNECT_OK) {
		return 0;
	}


	m_sys = finslib_tcp_connect(NULL, m_ip.c_str(), m_port, 0, 10, 0, 0, 1,0,&error_val, 6);
	if(m_sys == NULL || error_val != 0) {
		std::cout << "plc fins connect error!" << endl;
		m_sys = NULL;
		return -1;
	}

	cout << "sys: " << m_sys << endl;
	m_sys->plc_mode = FINS_MODE_CS;
	m_connectionStatus = CONNECT_OK;

	return 0;
}


void Plc::on_disconnect()
{
	//EventLooper::GetInstance().CancelTimer(m_detect);
	finslib_disconnect(m_sys);
	m_sys = NULL;
	m_connectionStatus = CONNECT_NO;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T> 
void convertFromString(T &value, const std::string &s) {
	std::stringstream ss(s);
	ss >> value;
}

PlcFins::PlcFins(string cmd_addr,string data_addr,Plc* plc)
	: m_cmdAddr(cmd_addr)
	, m_dataAddr(data_addr)
	, m_plc(plc)
{
	m_status = PLC_STATUS_IDEL;
	//m_plc->m_sys = NULL;
	struct timeval tv={5, 0};
	m_reconnect = EventLooper::GetInstance().ScheduleTimer(&tv, TF_FIRE_PERIODICALLY, this);

	struct timeval tv1={1, 0};
	m_valuePolling = EventLooper::GetInstance().ScheduleTimer(&tv1, TF_FIRE_PERIODICALLY, this);
}

PlcFins::~PlcFins()
{
	EventLooper::GetInstance().CancelTimer(m_reconnect);
}

void PlcFins::UpdateInfo(const char *buf,int len)
{
	Json::Reader reader;
	Json::Value root;

	string strret = buf;

	if (strret == "errorFrame" || strret == "errorData") {
		//EventLooper::GetInstance().CancelTimer(m_valuePolling);
		//cout << "##1 CancelTimer tid :" << m_valuePolling << endl;
		short cmd_ret = ERROR_RESULT;
		unsigned char *ret_cmd = (unsigned char *)(&cmd_ret);
		int ret = finslib_memory_area_write_word(m_plc->m_sys,m_cmdAddr.c_str(),ret_cmd,1);
		m_status = PLC_STATUS_IDEL;
		printf("Error data!\n");
		return;
	}

	//EventLooper::GetInstance().CancelTimer(m_valuePolling);
	//cout << "##2 CancelTimer tid :" << m_valuePolling << endl;
	if(!reader.parse(buf, root)){
		printf("JsonStringToStructData reader parse error.\n");
		return;
	}


	string valuestr;
	string value  = root["value"].asString();
	if(root["symbol"].asString() == "+" ){
		valuestr = root["value"].asString();

	} else {
		valuestr = "-" + trim(value);
	}
	//string valuestr = root["symbol"].asString() + root["value"].asString();
	printf("result:%s\n",valuestr.c_str());

	float val;
	convertFromString(val,valuestr);
	printf("val:%f\n",val);
	int ret = finslib_memory_area_write_int32( m_plc->m_sys, m_dataAddr.c_str(),(int *)(&val), 1 ) ;
	if(ret != FINS_RETVAL_SUCCESS){
		m_plc->on_disconnect();
		return;
	}

	short cmd_ret = 0;
	unsigned char *ret_cmd = (unsigned char *)(&cmd_ret);
	ret = finslib_memory_area_write_word(m_plc->m_sys,m_cmdAddr.c_str(),ret_cmd,1);
	cout << "***************   ret:    "  << ret << "value:" << val << endl;
	m_status = PLC_STATUS_IDEL;
}

int PlcFins::Connect()
{

	int ret;
	ret = m_plc->Connect();
	if (ret != 0) {
		return ret;
	}

	m_status = PLC_STATUS_IDEL;

	struct timeval tv={0, 200};
	m_detect = EventLooper::GetInstance().ScheduleTimer(&tv, TF_FIRE_PERIODICALLY, this);

	return 0;
}


void PlcFins::OnTimer(TimerID tid)
{

	int ret;
	//cout << "OnTimer " << tid << endl;
	if (tid == m_detect){
		if(m_status == PLC_STATUS_IDEL) {
			if (m_plc->m_sys){
				//cout << "m_detect" << endl;
				//cout << "sys: " << m_sys << "    m_cmdAddr: " << m_cmdAddr << endl; 
				
				ret = finslib_memory_area_read_word(m_plc->m_sys,m_cmdAddr.c_str(),m_cmd,1);
				unsigned char ch = m_cmd[0];
				m_cmd[0] = m_cmd[1];
				m_cmd[1] = ch;                   // da xiao duan tiao zheng.
				//printf("ret:%d\n",ret);
				if(ret != FINS_RETVAL_SUCCESS) {
					m_plc->on_disconnect();
					return;
				}
				short *cmd = (short *)m_cmd;
				if(*cmd == EBALANCE_QP) {
					cout << "----------- qu pi!" << endl;
					string QPcmd = "%ET";  //去皮
					m_Peripheral->WriteData(QPcmd);

					// qp cmd is not did anything,return OK
					short cmd_ret = 0;
					unsigned char *ret_cmd = (unsigned char *)(&cmd_ret);
					ret = finslib_memory_area_write_word(m_plc->m_sys,m_cmdAddr.c_str(),ret_cmd,1);
					
				} else if(*cmd == EBALANCE_CZ){
					cout << "---------- du shuju" << endl;
					string QPcmd = "%ER";  //读天平数
					m_Peripheral->WriteData(QPcmd);
					m_status = PLC_STATUS_BUSY;
					gettimeofday(&m_startTime,NULL);
					
					//cout << "## start tid :" << m_valuePolling << endl;
				} else {
					//cout << "----- cmd error!" << endl;
					//short error_cmd = ERROR_CMD;
					//unsigned char *ret_cmd = (unsigned char *)(&error_cmd);
					//ret = finslib_memory_area_write_word(m_sys,m_cmdAddr.c_str(),ret_cmd,1);
					//if(ret != FINS_RETVAL_SUCCESS) {
					//	on_disconnect();
					//	return;
					//}
				}
			}
		} else if (m_status == PLC_STATUS_BUSY) {
			struct timeval tv;
			gettimeofday(&tv,NULL);

			int timeUse = (tv.tv_sec*1000 + tv.tv_usec/1000) - (m_startTime.tv_sec*1000 + m_startTime.tv_usec/1000);
			if ( timeUse > 15000) {
				//EventLooper::GetInstance().CancelTimer(m_valuePolling);
				//cout << "##3 CancelTimer tid :" << m_valuePolling << endl;
				short error_cmd = ERROR_TIMEOUT;
				unsigned char *ret_cmd = (unsigned char *)(&error_cmd);
				ret = finslib_memory_area_write_word(m_plc->m_sys,m_cmdAddr.c_str(),ret_cmd,1);
				if(ret != FINS_RETVAL_SUCCESS) {
					m_plc->on_disconnect();
					return;
				}
				m_status = PLC_STATUS_IDEL;
			}
		}

	} else if (tid == m_reconnect) {
		//cout << "m_reconnect_timer" << endl;
		if ( m_plc->m_sys == NULL) {
			cout << "m_reconnect" << endl;
			Connect();
		}
	} else if (tid == m_valuePolling) {
		if(m_status == PLC_STATUS_BUSY) {
			cout << "## value polling, tid :" << tid << endl;
			string QPcmd = "%ER";  //读天平数
			m_Peripheral->WriteData(QPcmd);
		}
	}
}


