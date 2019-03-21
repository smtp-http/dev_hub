#include <string.h>
#include <iostream>
#include <json/json.h>
#include <time.h>
#include <linux/input.h>
#include "key_code.h"
#include "Scanner.h"

using namespace lux;
using namespace std;



KeyenceScanner::KeyenceScanner(IUpdateSink *updater,string dev)
{
	m_updater = updater;
	printf("0x%x\n",m_updater);
	m_devName = dev;
}
KeyenceScanner::~KeyenceScanner()
{

}


int KeyenceScanner::OnData(char *data,int len)
{
	// TODO: data analysis
	time_t t;
    time(&t);
	printf("%s : %d\n",__FILE__,__LINE__);
	string code(data);
	//strncpy(code.c_str(),data,len);

	int ti = (int)t;
	
	Json::Value root;
	root["dev"] = m_devName;
	root["time"] = ti;//(long int)t;
	root["code"] = code;
	printf("            : %d\n",__LINE__);

	std::string out = root.toStyledString();
	printf("out:%s\n",out.c_str());
	
	if(m_updater){printf("            : %d   0x%x\n",__LINE__,m_updater);
		m_updater->UpdateInfo(out.data(),out.length());
	}
	return 0;
}


HandhelScanner::HandhelScanner(IUpdateSink *updater,string dev)
{
	m_updater = updater;
	m_devName = dev;
}
HandhelScanner::~HandhelScanner()
{

}


int HandhelScanner::OnData(char *data,int len)
{
	// TODO: data analysis
	int j,sendLen;
	static int i = 0;
	int lSurplus = len;
	//memset(barBuf,0,100);
	static char barBuf[100];
	Json::Value root;
	time_t t;
	printf("%s : %d\n",__FILE__,__LINE__);
	struct input_event inputEvent;

	while(lSurplus >= sizeof(struct input_event)){
		memcpy(&inputEvent,data + len - lSurplus,sizeof(struct input_event));
		lSurplus = lSurplus - sizeof(struct input_event);
		if(inputEvent.type == 1) {

			if(inputEvent.code == 28){

				barBuf[i ++] = '\n';
				sendLen = i/2;
				
				for(j = 0;j < sendLen;j ++){
					printf("%c",barBuf[j]);
					barBuf[j+1] = barBuf[j*2 + 2];
				}

				barBuf[sendLen] = 0;
				
    			time(&t);
				
				string code(barBuf);

				root["dev"] = m_devName;
				root["time"] = (int)t;//(long int)t;
				root["code"] = code;

				string out = root.toStyledString();
				
				if(m_updater && sendLen > 0){
					m_updater->UpdateInfo(out.data(),out.length());
				}
				
				i = 0;
				memset(barBuf,0,100);
			} else if(inputEvent.code > sizeof(key_ascii_map)){
				//barBuf[i ++] = '#';
				printf(" +%d+ ",inputEvent.code);
					//printf("error code: %d\n",inputEvent.code);
					//continue;
			} else {
				char c = key_ascii_map[inputEvent.code];
				if(c != 0){
					barBuf[i ++] = c;
						//printf("%c -%d- ",c,i);
				} else {
					//barBuf[i ++] = '*';
					//printf(" ='%d'= ",inputEvent.code);
				}
			}

		} 
	}
	
	return 0;
}



