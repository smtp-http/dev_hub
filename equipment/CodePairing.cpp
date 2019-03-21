#include <json/json.h>
#include <sys/time.h>
#include <string.h>
#include "CodePairing.h"

using namespace lux;
using namespace std;


CodePairing::CodePairing()
{
	m_tcpTransfer = NULL;
	m_lastRecord.IsUseful = 0;
}


CodePairing::~CodePairing()
{

}

void CodePairing::UpdateInfo(const char * buf, int len)
{
	// TODO: Pairing barcode
	printf("%s : %d\n",__FILE__,__LINE__);
	struct timeval tv;
    gettimeofday(&tv,NULL);
	string lastDev,curDev;

	//printf("-%d- ",__LINE__);
	
	Json::Reader reader;
	Json::Value valueLast,valueCur;

	long long currentTimeUs = tv.tv_sec*1000 + tv.tv_usec/1000;

	string curInfo = buf;
	if (reader.parse(curInfo, valueCur)) {
		if(strncmp("ERROR\\r\\n",valueCur["code"].asString().c_str(),5) == 0){
			Json::Value root;
			root["errorCode"] = __ERROR_ERRORRESULT;
			root["errorDetail"] = curInfo.c_str();

			string out = root.toStyledString();
			//printf("%s\n",out.c_str());
			if(m_tcpTransfer && out.length() > 0){
				m_tcpTransfer->UpdateInfo(out.c_str(),out.length());
			}
		}
		return;
	} else {
		return;
	}
	

	if(m_lastRecord.IsUseful) {
		if (reader.parse(m_lastRecord.Code, valueLast)) {//printf("-%d- ",__LINE__);
			lastDev = valueLast["dev"].asString();
			//string curInfo = buf;
			//if (reader.parse(curInfo, valueCur)) {//printf("-%d- ",__LINE__);
				curDev = valueCur["dev"].asString();
				if (lastDev == curDev) {
					// TODO: send error
					printf("-- dev repeat! error!\n");
					Json::Value root;
					root["errorCode"] = __ERROR_NOPAIRING;
					root["errorDetail"] = curInfo.c_str();

					string out = root.toStyledString();
					//printf("%s\n",out.c_str());

					if(m_tcpTransfer && out.length() > 0){
						m_tcpTransfer->UpdateInfo(out.c_str(),out.length());
					}
					
				} else {//printf("-%d- ",__LINE__);
					if(currentTimeUs -  m_lastRecord.LastTimeUs > 3000) { //1秒
						//TODO: send timeout err
						Json::Value root;
						root["errorCode"] = __ERROR_NOPAIRING;
						root["errorDetail"] = curInfo.c_str();

						string out = root.toStyledString();
						//printf("%s\n",out.c_str());
						if(m_tcpTransfer && out.length() > 0){
							m_tcpTransfer->UpdateInfo(out.c_str(),out.length());
						}
						
						//printf("-%d- ",__LINE__);
						m_lastRecord.Code = buf;
						m_lastRecord.LastTimeUs = currentTimeUs;
						m_lastRecord.IsUseful = 1;

					} else {
						printf("Code pairing succeed!\n");
						Json::Value root; 
						Json::Value arrayObj;
						Json::Value item1,item2;
						
						item1["dev"] = curDev;
						item1["code"] = valueCur["code"].asString();

						arrayObj.append(item1);
						
						item2["dev"] = lastDev;
						item2["code"] = valueLast["code"].asString();

						root["errorCode"] = __SUCCEED;
						root["result"] = arrayObj;
						
						//printf("-%d- ",__LINE__);

						arrayObj.append(item2);

						string out = arrayObj.toStyledString();

						if(m_tcpTransfer && out.length() > 0){
							m_tcpTransfer->UpdateInfo(out.c_str(),out.length());
						}

						m_lastRecord.IsUseful = 0;

						return;
					}
				}
			//} else {
			//	return;
			//}
			
		} else {
		printf("-%d- ",__LINE__);
			m_lastRecord.IsUseful = 0;
		}
	} else {
		printf("-%d- ",__LINE__);
		m_lastRecord.LastTimeUs = currentTimeUs;
		m_lastRecord.Code = buf;
		printf("len: %d    strlen:%d\n",len,m_lastRecord.Code.length());
		m_lastRecord.IsUseful = 1;
	}
	
}

void CodePairing::SetTcpTransfer(IUpdateSink * transfer)
{
	m_tcpTransfer = transfer;
}


CodePairing& CodePairing::GetInstance()
{
	static  CodePairing instance_;
	return instance_; 
}

/////////////////////////////////// test ///////////////////////////////////

void CodePairingTest(IUpdateSink *transfer)
{
	CodePairing *cp = &CodePairing::GetInstance();
	cp->SetTcpTransfer(transfer);

//	string out = "hello";
	
//	cp->UpdateInfo(const char * buf, int len)
}


