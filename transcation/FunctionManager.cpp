#include "FunctionManager.h"
#include <string>
#include <string.h>
#include <json/json.h>
#include "connbase.h"
#include "Peripheral.h"
#include "Uart.h"
#include <iostream>

#include "PlcFins.h"

using namespace lux;
using namespace std;


Equipment *BuildElecBalance(int equipNum,IUpdateSink *updater);

Equipment *BuildSolderCtrl(int equipNum,IUpdateSink *updater);
Equipment *BuildGlueDispenser(int equipNum,IUpdateSink *updater);

FunctionManager::FunctionManager()
{

}

FunctionManager::~FunctionManager()
{

}

FunctionManager& FunctionManager::GetInstance()
{
	static  FunctionManager instance_;
	return instance_; 
}


void FunctionManager::Update(const Json::Value &value)
{

	if(value["functions"].isNull()) {
		cout << "function is null" << endl;
		return;
	}


	Json::Value arrayFunction  = value["functions"];

	for(unsigned int i = 0; i < arrayFunction.size(); i++) {

		string funcName;

		if ( !arrayFunction[i]["name"].isNull() && arrayFunction[i]["name"].isString()) {
			funcName = arrayFunction[i]["name"].asString();
		}


		if ( funcName == "balance") {
			
		} else if (funcName == "solder") {
			
		} else if (funcName == "GlueDispenser") {
			
		} else {
			
			continue;
		}


		Json::Value boss = arrayFunction[i]["boss"];
		string bossName;
		if(!boss.isNull() && boss.isString()){
			bossName = boss.asString(); 
		} else {
			//log.Log(true,LOGGER_ERROR,"arrayEquipment[%d] is null or is not string type, [%s][%s][%d]",j,__FILE__,__PRETTY_FUNCTION__,__LINE__);
			continue;
		}

		Json::Value plcValue = value[bossName];
		if(plcValue.isNull()){
			printf("Can not find this plc %s\n",bossName.c_str());
			continue;
		}

		string plc_ip;
		if(!plcValue["ip"].isNull()  && plcValue["ip"].isString()) {
			plc_ip = plcValue["ip"].asString();
		} else {
			printf("Ip error!\n");
			continue;
		}

		int plc_port;
		if(!plcValue["port"].isNull()  && plcValue["port"].isInt()) {
			plc_port = plcValue["port"].asInt();
		} else {
			printf("port error!\n");
			continue;
		}


		string plc_cmdAddr;
		if(!plcValue["cmd_addr"].isNull()  && plcValue["cmd_addr"].isString()) {
			plc_cmdAddr = plcValue["cmd_addr"].asString();
		} else {
			printf("cmd_addr error!\n");
			continue;
		}


		string plc_dataAddr;
		if(!plcValue["data_addr"].isNull()  && plcValue["data_addr"].isString()) {
			plc_dataAddr = plcValue["data_addr"].asString();
		} else {
			printf("data_addr error!\n");
			continue;
		}

		PlcFins *plc = new PlcFins(plc_ip,plc_port,plc_cmdAddr,plc_dataAddr);

		
		Json::Value peripheral = arrayFunction[i]["peripheral"];

		string peripheralName;
		if(!peripheral.isNull() && peripheral.isString()){
			peripheralName = peripheral.asString(); 
		} else {
			//log.Log(true,LOGGER_ERROR,"arrayEquipment[%d] is null or is not string type, [%s][%s][%d]",j,__FILE__,__PRETTY_FUNCTION__,__LINE__);
			continue;
		}
		if (strncmp(peripheralName.c_str(),"uart",4) == 0) {
			Json::Value uartValue = value[peripheralName];
				
			string devName;
			if(!uartValue["devName"].isNull() && uartValue["devName"].isString()){
				devName = uartValue["devName"].asString();
			} else {
				//log.Log(true,LOGGER_ERROR,"value[%s] is null or is not string type, [%s][%s][%d]",peripheralName.c_str(),__FILE__,__PRETTY_FUNCTION__,__LINE__);
				continue;
			}
				
			int baudRate;
			if(!uartValue["BaudRate"].isNull() && uartValue["BaudRate"].isInt()) {
				baudRate = uartValue["BaudRate"].asInt();
			} else {
				//log.Log(true,LOGGER_ERROR,"value[%s][\"BaudRate\"] is null or is not string type, [%s][%s][%d]",peripheralName.c_str(),__FILE__,__PRETTY_FUNCTION__,__LINE__);
				continue;
			}


			Peripheral *uart = new Uart(devName,baudRate);

				

			int equipNum;
			if(!uartValue["equipNum"].isNull() && uartValue["equipNum"].isInt()) {
				equipNum = uartValue["equipNum"].asInt();
			} else {
				//log.Log(true,LOGGER_ERROR,"value[%s][\"equipNum\"] is null or is not int type, [%s][%s][%d]",peripheralName.c_str(),__FILE__,__PRETTY_FUNCTION__,__LINE__);
				continue;
			}
				
			Equipment *equipment = NULL;


				
			if (funcName == "solder") {
				equipment = BuildSolderCtrl(equipNum,plc);
			} else if ( funcName == "balance" ) {
				equipment = BuildElecBalance(equipNum,plc);
			} else if (funcName == "GlueDispenser") {
				cout << "==========================    glue +++++++++++++++++++++" << endl;
				equipment = BuildGlueDispenser(equipNum,plc);
				//printf("-------- equitment: 0x%x\n",equipment);
			}

			uart->SetEquipment(equipment);

			equipment->SetPeripheral(uart);
				
			uart->OpenPeripheral();

			uart->SetEquipmentName(funcName);

			uart->SetEquipmentNo(equipNum);
				
			plc->SetPeripheral(uart);
		} else if (strncmp(peripheralName.c_str(),"usb",3) == 0) {
				
		} else {
			printf("%s:%d    equip name:%s\n",__FILE__,__LINE__,peripheralName.c_str());
		}
	}
}


