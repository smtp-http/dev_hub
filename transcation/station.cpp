#include "station.h"
#include "tinyxml.h"
#include <iostream>


int GetEventProfile(StationEventProfile_t *event,TiXmlNode *StationEventProfileNode)
{

	TiXmlElement* profileElement = StationEventProfileNode->ToElement();

	if(!strcmp(profileElement->Value(),"Name")) {
		event->Name = profileElement->GetText();
	} else if(!strcmp(profileElement->Value(),"Enable")) {
		string EnableStr = profileElement->GetText();
		if(!strcmp(EnableStr.c_str(),"true")) {
			event->Enable = true;
		} else if (!strcmp(EnableStr.c_str(),"false")) {
			event->Enable = false;
		}
	} else if(!strcmp(profileElement->Value(),"UpdateTimeTicks")) {
		string ticks = profileElement->GetText();
		event->UpdateTimeTicks = strtoull (ticks.c_str(), NULL, 0);
	} else if(!strcmp(profileElement->Value(),"Flag")) {
		event->Flag = profileElement->GetText();
	} else if(!strcmp(profileElement->Value(),"Action")) { 
		event->Action = profileElement->GetText();
	} else if(!strcmp(profileElement->Value(),"Properties")) {
		event->Properties = profileElement->GetText();
	}else if(!strcmp(profileElement->Value(),"PlcBlockAddress")) {
		event->PlcBlockAddress = profileElement->GetText();
	}else if(!strcmp(profileElement->Value(),"PlcBlockSize")) {
		string size = profileElement->GetText();
		event->PlcBlockSize = strtoul(size.c_str(), NULL, 10);
	}else if(!strcmp(profileElement->Value(),"EapBlockAddress")) {
		event->EapBlockAddress = profileElement->GetText();
	}else if(!strcmp(profileElement->Value(),"EapBlockSize")) {
		string addr = profileElement->GetText();
		event->EapBlockSize = strtoul(addr.c_str(), NULL, 10);
	} else{
		return -1;
	}

	return 0;
}

	string Name;
	string Remark;
	bool Enable;
	unsigned long long UpdateTimeTicks;
	bool Status;
	string Level;
	unsigned int Duration;
	unsigned int WordOffset;
	unsigned int BitOffset;

int GetAlarmCodeList(PlcAlarmInfo_t* alarm,TiXmlNode* PlcAlarmInfoNode)
{
	TiXmlElement* alarmInfoElement = PlcAlarmInfoNode->ToElement();

	if(!strcmp(alarmInfoElement->Value(),"Name")) {
		alarm->Name = alarmInfoElement->GetText();
	} else if(!strcmp(alarmInfoElement->Value(),"Remark")) {
		alarm->Remark = alarmInfoElement->GetText();
	} else if(!strcmp(alarmInfoElement->Value(),"Enable")) {
		string EnableStr = alarmInfoElement->GetText();
		if(!strcmp(EnableStr.c_str(),"true")) {
			alarm->Enable = true;
		} else if (!strcmp(EnableStr.c_str(),"false")) {
			alarm->Enable = false;
		}
	} else if(!strcmp(alarmInfoElement->Value(),"UpdateTimeTicks")) {
		string ticks = alarmInfoElement->GetText();
		alarm->UpdateTimeTicks = strtoull (ticks.c_str(), NULL, 0);
	} else if(!strcmp(alarmInfoElement->Value(),"Status")) {
		string EnableStr = alarmInfoElement->GetText();
		if(!strcmp(EnableStr.c_str(),"true")) {
			alarm->Status = true;
		} else if (!strcmp(EnableStr.c_str(),"false")) {
			alarm->Status = false;
		}
	} else if(!strcmp(alarmInfoElement->Value(),"Level")) { 
		alarm->Level = alarmInfoElement->GetText();
	}else if(!strcmp(alarmInfoElement->Value(),"Duration")) {
		string dur = alarmInfoElement->GetText();
		alarm->Duration = strtoul(dur.c_str(), NULL, 10);
	}else if(!strcmp(alarmInfoElement->Value(),"WordOffset")) {
		string off = alarmInfoElement->GetText();
		alarm->WordOffset = strtoul(off.c_str(), NULL, 10);
	}else if(!strcmp(alarmInfoElement->Value(),"BitOffset")) {
		string off = alarmInfoElement->GetText();
		alarm->BitOffset = strtoul(off.c_str(), NULL, 10);
	} else{
		return -1;
	}

	return 0;
}



vector<LineMachine_t*> *GetLineMachineList(TiXmlNode *MachineNode)
{
	//vector<StationEventProfile_t*> *vs = new(vector<StationEventProfile_t*>);
	vector<LineMachine_t*>  *lm = new(vector<LineMachine_t*>);

	
	//TiXmlNode* MachineNode = NULL;
	TiXmlNode* LineMachineNode = NULL;
	TiXmlNode* MainDeviceProfileNode = NULL;

	TiXmlNode* SettingsNode = NULL;

	TiXmlNode* EventsNode = NULL;
	TiXmlNode* AlarmCodeListNode = NULL;
	TiXmlNode* StationEventProfileNode = NULL;
	TiXmlNode* PlcAlarmInfoNode = NULL;
	TiXmlNode* NormalSettingListNode = NULL;
	TiXmlNode* PlcMemoryProfileInfoNode = NULL;
	TiXmlNode* StationsNode = NULL;
	TiXmlNode* LineStationNode = NULL;



	TiXmlElement* LineMachineElement = NULL;
	TiXmlElement* MainDeviceProfileElement = NULL;
	//TiXmlElement* todoElement = 0;
	//TiXmlElement* itemElement = 0;
	TiXmlElement* SettingsElement = NULL;

	TiXmlElement* EventsElement = NULL;
	TiXmlElement* AlarmCodeListElement = NULL;
	TiXmlElement* StationEventProfileElement = NULL;
	TiXmlElement* PlcAlarmInfoElement = NULL;
	TiXmlElement* NormalSettingListElement = NULL;
	TiXmlElement* PlcMemoryProfileInfoElement = NULL;
	TiXmlElement* StationsElement = NULL;
	TiXmlElement* LineStationElement = NULL;


	for(;MachineNode != NULL;MachineNode = MachineNode->NextSibling()) {
		//printf("MachineNode: 0x%x   %s\n",MachineNode,MachineNode->Value());
		if(!strcmp(MachineNode->Value(),"LineMachine")) {
			LineMachine_t *machine = new(LineMachine_t);
			LineMachineElement = MachineNode->ToElement();
			LineMachineNode = LineMachineElement->FirstChildElement();
			for(;LineMachineNode != NULL;LineMachineNode = LineMachineNode->NextSibling()) {
				//printf("LineMachineNode: 0x%x   %s\n",LineMachineNode,LineMachineNode->Value()); 
				if(!strcmp(LineMachineNode->Value(),"Name")){
					machine->Name = LineMachineNode->ToElement()->GetText();
					
				} else if(!strcmp(LineMachineNode->Value(),"Enable")) {
					if(!strcmp(LineMachineNode->ToElement()->GetText(),"true")){
						machine->Enable = true;
					} else if(!strcmp(LineMachineNode->ToElement()->GetText(),"false")){
						machine->Enable = false;
					} else {
						printf("Enable text error: %s\n", LineMachineNode->ToElement()->GetText());
					}
				} else if(!strcmp(LineMachineNode->Value(),"UpdateTimeTicks")) {
					string ticks = LineMachineNode->ToElement()->GetText();
					machine->UpdateTimeTicks = strtoull (ticks.c_str(), NULL, 0);
					

				} else if(!strcmp(LineMachineNode->Value(),"MainDeviceProfile")) {
					MainDeviceProfileElement = LineMachineNode->ToElement();
					MainDeviceProfileNode = MainDeviceProfileElement->FirstChildElement();
					for(;MainDeviceProfileNode != NULL;MainDeviceProfileNode = MainDeviceProfileNode->NextSibling()) {
						//printf("MainDeviceProfileNode: 0x%x   %s\n",MainDeviceProfileNode,MainDeviceProfileNode->Value()); 
						if(!strcmp(MainDeviceProfileNode->Value(),"Name")){
							machine->mainDeviceProfile.Name = MainDeviceProfileNode->ToElement()->GetText();
							
						} else if(!strcmp(MainDeviceProfileNode->Value(),"Enable")) {
							if(!strcmp(MainDeviceProfileNode->ToElement()->GetText(),"true")){
								machine->mainDeviceProfile.Enable = true;
							} else if(!strcmp(MainDeviceProfileNode->ToElement()->GetText(),"false")){
								machine->mainDeviceProfile.Enable = false;
							} else {
								printf("Enable text error: %s\n", MainDeviceProfileNode->ToElement()->GetText());
							}
							
						} else if(!strcmp(MainDeviceProfileNode->Value(),"UpdateTimeTicks")) {
							string ticks = MainDeviceProfileNode->ToElement()->GetText();
							machine->mainDeviceProfile.UpdateTimeTicks = strtoull (ticks.c_str(), NULL, 0);

						} else if(!strcmp(MainDeviceProfileNode->Value(),"DeviceType")) {
							machine->mainDeviceProfile.DeviceType = MainDeviceProfileNode->ToElement()->GetText();
							
						} else if(!strcmp(MainDeviceProfileNode->Value(),"ProtocolInfo")) {
							TiXmlNode* ProtocolNode;
							TiXmlElement* ProtocolElement;

							ProtocolElement = MainDeviceProfileNode->ToElement();
							ProtocolNode = ProtocolElement->FirstChildElement();
							for(;ProtocolNode != NULL;ProtocolNode = ProtocolNode->NextSibling()) {
								if(!strcmp(ProtocolNode->Value(),"Protocol")){
									machine->mainDeviceProfile.protocolInfo.Protocol = ProtocolNode->ToElement()->GetText();
								} else if(!strcmp(ProtocolNode->Value(),"StationID")){
									string StationID = ProtocolNode->ToElement()->GetText();
									machine->mainDeviceProfile.protocolInfo.stationId = strtoul(StationID.c_str(), NULL, 10);
								}
							}
						} else if(!strcmp(MainDeviceProfileNode->Value(),"PortName")) {
							machine->mainDeviceProfile.serialPara.portName = MainDeviceProfileNode->ToElement()->GetText();
						} else if(!strcmp(MainDeviceProfileNode->Value(),"BaudRate")) {
							string baud = MainDeviceProfileNode->ToElement()->GetText();
							machine->mainDeviceProfile.serialPara.baud = strtoul(baud.c_str(), NULL, 10);
						} else if(!strcmp(MainDeviceProfileNode->Value(),"DataBits")) {
							string DataBits = MainDeviceProfileNode->ToElement()->GetText();
							machine->mainDeviceProfile.serialPara.data_bit = strtoul(DataBits.c_str(), NULL, 10);
						} else if(!strcmp(MainDeviceProfileNode->Value(),"StopBits")) {
							string StopBits = MainDeviceProfileNode->ToElement()->GetText();
							if(StopBits == "One") 
								machine->mainDeviceProfile.serialPara.stop_bit = 1;
							else if(StopBits == "Two") 
								machine->mainDeviceProfile.serialPara.stop_bit = 2;
						} else if(!strcmp(MainDeviceProfileNode->Value(),"Parity")) {
							string Parity = MainDeviceProfileNode->ToElement()->GetText();
							if(Parity == "None")
								machine->mainDeviceProfile.serialPara.parity = 'N';
							else if (Parity == "Odd") 
								machine->mainDeviceProfile.serialPara.parity = 'O';
							else if (Parity == "Even")
								machine->mainDeviceProfile.serialPara.parity = 'E';
							else if (Parity == "Mark")
								machine->mainDeviceProfile.serialPara.parity = 'M';
							else if (Parity == "Space")
								machine->mainDeviceProfile.serialPara.parity = 'S';
							else 
								printf("%s:%d  Parity is error!\n",__FILE__,__LINE__);

						} else if(!strcmp(MainDeviceProfileNode->Value(),"Priority")) {
							machine->mainDeviceProfile.Priority = MainDeviceProfileNode->ToElement()->GetText();
							
						} else if(!strcmp(MainDeviceProfileNode->Value(),"ProcessTimeout")) {
							string timeout = MainDeviceProfileNode->ToElement()->GetText();
							machine->mainDeviceProfile.ProcessTimeout = strtoul(timeout.c_str(), NULL, 10);

						} else if(!strcmp(MainDeviceProfileNode->Value(),"Port")) {
							string Port = MainDeviceProfileNode->ToElement()->GetText();
							machine->mainDeviceProfile.Port = strtoul(Port.c_str(), NULL, 10);
							
						} else if(!strcmp(MainDeviceProfileNode->Value(),"WriteBufferSize")) {
							string WriteBufferSize = MainDeviceProfileNode->ToElement()->GetText();
							machine->mainDeviceProfile.WriteBufferSize = strtoul(WriteBufferSize.c_str(), NULL, 10);
							
						} else if(!strcmp(MainDeviceProfileNode->Value(),"WriteTimeout")) {
							string WriteTimeout = MainDeviceProfileNode->ToElement()->GetText();
							machine->mainDeviceProfile.WriteTimeout = strtoul(WriteTimeout.c_str(), NULL, 10);
							
						} else if(!strcmp(MainDeviceProfileNode->Value(),"WriteRetryCount")) {
							string WriteRetryCount = MainDeviceProfileNode->ToElement()->GetText();
							machine->mainDeviceProfile.WriteRetryCount = strtoul(WriteRetryCount.c_str(), NULL, 10);
							
						} else if(!strcmp(MainDeviceProfileNode->Value(),"ReadTimeout")) {
							string ReadTimeout = MainDeviceProfileNode->ToElement()->GetText();
							machine->mainDeviceProfile.ReadTimeout = strtoul(ReadTimeout.c_str(), NULL, 10);
							
						} else if(!strcmp(MainDeviceProfileNode->Value(),"ReadBufferSize")) {
							string ReadBufferSize = MainDeviceProfileNode->ToElement()->GetText();
							machine->mainDeviceProfile.ReadBufferSize = strtoul(ReadBufferSize.c_str(), NULL, 10);
							
						} else if(!strcmp(MainDeviceProfileNode->Value(),"KeepAliveInterval")) {
							string KeepAliveInterval = MainDeviceProfileNode->ToElement()->GetText();
							machine->mainDeviceProfile.KeepAliveInterval = strtoul(KeepAliveInterval.c_str(), NULL, 10);
							
						} else if(!strcmp(MainDeviceProfileNode->Value(),"SockType")) {
							machine->mainDeviceProfile.SockType = MainDeviceProfileNode->ToElement()->GetText();
							
						} else if(!strcmp(MainDeviceProfileNode->Value(),"IsQueueMode")) {
							if(!strcmp(MainDeviceProfileNode->ToElement()->GetText(),"true")){
								machine->mainDeviceProfile.IsQueueMode = true;
							} else if(!strcmp(MainDeviceProfileNode->ToElement()->GetText(),"false")){
								machine->mainDeviceProfile.IsQueueMode = false;
							} else {
								printf("Enable text error: %s\n", MainDeviceProfileNode->ToElement()->GetText());
							}
							
						} else if(!strcmp(MainDeviceProfileNode->Value(),"RoutePort")) {
							string RoutePort = MainDeviceProfileNode->ToElement()->GetText();
							machine->mainDeviceProfile.RoutePort = strtoul(RoutePort.c_str(), NULL, 10);
							
						} else if(!strcmp(MainDeviceProfileNode->Value(),"AutoReconnectInterval")) {
							string AutoReconnectInterval = MainDeviceProfileNode->ToElement()->GetText();
							machine->mainDeviceProfile.AutoReconnectInterval = strtoul(AutoReconnectInterval.c_str(), NULL, 10);
							
						} else if(!strcmp(MainDeviceProfileNode->Value(),"IpAddress")) {
							machine->mainDeviceProfile.IpAddress = MainDeviceProfileNode->ToElement()->GetText();
							cout << "+++++++ ip address:" << machine->mainDeviceProfile.IpAddress << endl;
						} else if(!strcmp(MainDeviceProfileNode->Value(),"PollFrequency")) {
							string PollFrequency = MainDeviceProfileNode->ToElement()->GetText();
							machine->mainDeviceProfile.PollFrequency = strtoul(PollFrequency.c_str(), NULL, 10);
							
						} else if(!strcmp(MainDeviceProfileNode->Value(),"HeartBeatFrequency")) {
							string HeartBeatFrequency = MainDeviceProfileNode->ToElement()->GetText();
							machine->mainDeviceProfile.HeartBeatFrequency = strtoul(HeartBeatFrequency.c_str(), NULL, 10);
							

						} else if(!strcmp(MainDeviceProfileNode->Value(),"NormalSettings")) {
							SettingsElement = MainDeviceProfileNode->ToElement();
							SettingsNode = SettingsElement->FirstChildElement();
							for(;SettingsNode != NULL;SettingsNode = SettingsNode->NextSibling()) {
								if (!strcmp(SettingsNode->Value(),"Flag")) {
									machine->mainDeviceProfile.NormalSettings.Flag = SettingsNode->ToElement()->GetText();
									
								} else if (!strcmp(SettingsNode->Value(),"BeginAddress")) {
									machine->mainDeviceProfile.NormalSettings.BeginAddress = SettingsNode->ToElement()->GetText();
									
								} else if (!strcmp(SettingsNode->Value(),"DataSize")) {
									string DataSize = SettingsNode->ToElement()->GetText();
									machine->mainDeviceProfile.NormalSettings.DataSize = strtoul(DataSize.c_str(), NULL, 10);
									
								}
							}
						} else if(!strcmp(MainDeviceProfileNode->Value(),"MotorSettings")) {
							SettingsElement = MainDeviceProfileNode->ToElement();
							SettingsNode = SettingsElement->FirstChildElement();
							for(;SettingsNode != NULL;SettingsNode = SettingsNode->NextSibling()) {
								if (!strcmp(SettingsNode->Value(),"Flag")) {
									machine->mainDeviceProfile.MotorSettings.Flag = SettingsNode->ToElement()->GetText();
									
								} else if (!strcmp(SettingsNode->Value(),"BeginAddress")) {
									machine->mainDeviceProfile.MotorSettings.BeginAddress = SettingsNode->ToElement()->GetText();
									
								} else if (!strcmp(SettingsNode->Value(),"DataSize")) {
									string DataSize = SettingsNode->ToElement()->GetText();
									machine->mainDeviceProfile.MotorSettings.DataSize = strtoul(DataSize.c_str(), NULL, 10);
									
								}
							}
						} else if(!strcmp(MainDeviceProfileNode->Value(),"PlcDataBlock")) {
							SettingsElement = MainDeviceProfileNode->ToElement();
							SettingsNode = SettingsElement->FirstChildElement();
							for(;SettingsNode != NULL;SettingsNode = SettingsNode->NextSibling()) {
								if (!strcmp(SettingsNode->Value(),"Flag")) {
									machine->mainDeviceProfile.PlcDataBlock.Flag = SettingsNode->ToElement()->GetText();
									
								} else if (!strcmp(SettingsNode->Value(),"BeginAddress")) {
									machine->mainDeviceProfile.PlcDataBlock.BeginAddress = SettingsNode->ToElement()->GetText();
									
								} else if (!strcmp(SettingsNode->Value(),"DataSize")) {
									string DataSize = SettingsNode->ToElement()->GetText();
									machine->mainDeviceProfile.PlcDataBlock.DataSize = strtoul(DataSize.c_str(), NULL, 10);
									
								}
							}
						} else if(!strcmp(MainDeviceProfileNode->Value(),"EapDataBlock")) {
							SettingsElement = MainDeviceProfileNode->ToElement();
							SettingsNode = SettingsElement->FirstChildElement();
							for(;SettingsNode != NULL;SettingsNode = SettingsNode->NextSibling()) {
								if (!strcmp(SettingsNode->Value(),"Flag")) {
									machine->mainDeviceProfile.EapDataBlock.Flag = SettingsNode->ToElement()->GetText();
									
								} else if (!strcmp(SettingsNode->Value(),"BeginAddress")) {
									machine->mainDeviceProfile.EapDataBlock.BeginAddress = SettingsNode->ToElement()->GetText();
									
								} else if (!strcmp(SettingsNode->Value(),"DataSize")) {
									string DataSize = SettingsNode->ToElement()->GetText();
									machine->mainDeviceProfile.EapDataBlock.DataSize = strtoul(DataSize.c_str(), NULL, 10);
									
								}
							}
						} else if(!strcmp(MainDeviceProfileNode->Value(),"Events")) {
							EventsElement = MainDeviceProfileNode->ToElement();
							EventsNode = EventsElement->FirstChildElement();
							for(;EventsNode != NULL;EventsNode = EventsNode->NextSibling()) {
								////printf("=== EventsNode: 0x%x   %s\n",EventsNode,EventsNode->Value()); 
								if(!strcmp(EventsNode->Value(),"StationEventProfile")) {
									StationEventProfileElement = EventsNode->ToElement();
									StationEventProfileNode = StationEventProfileElement->FirstChildElement();
									StationEventProfile_t *ev_profile = new(StationEventProfile_t);
									for(;StationEventProfileNode != NULL;StationEventProfileNode = StationEventProfileNode->NextSibling()) {
										////printf("==++++++= StationEventProfileNode: 0x%x   %s***** ==== ***** \n",StationEventProfileNode,StationEventProfileNode->Value()); 
										// TODO: ======= create station event profile =============
										GetEventProfile(ev_profile,StationEventProfileNode);
									}
									if(!strcmp(ev_profile->Properties.c_str(),"NoPlcEvent")){
										delete ev_profile;
									} else {
										machine->mainDeviceProfile.mainEvents.push_back(ev_profile);
									}
								}
							}
						} else if(!strcmp(MainDeviceProfileNode->Value(),"AlarmCodeList")){
							AlarmCodeListElement = MainDeviceProfileNode->ToElement();
							AlarmCodeListNode = AlarmCodeListElement->FirstChildElement();
							for(;AlarmCodeListNode != NULL;AlarmCodeListNode = AlarmCodeListNode->NextSibling()) {
								//printf("AlarmCodeListNode: 0x%x   %s\n",AlarmCodeListNode,AlarmCodeListNode->Value()); 
								if(!strcmp(AlarmCodeListNode->Value(),"PlcAlarmInfo")) {
									PlcAlarmInfoElement = AlarmCodeListNode->ToElement();
									PlcAlarmInfoNode = PlcAlarmInfoElement->FirstChildElement();
									PlcAlarmInfo_t* plcAlarmInfo = new(PlcAlarmInfo_t);
									for(;PlcAlarmInfoNode != NULL;PlcAlarmInfoNode = PlcAlarmInfoNode->NextSibling()) {
										GetAlarmCodeList(plcAlarmInfo,PlcAlarmInfoNode);
									}

									machine->mainDeviceProfile.AlarmCodeList.push_back(plcAlarmInfo);
								}
							}
						} else if(!strcmp(MainDeviceProfileNode->Value(),"NormalSettingList")){
							NormalSettingListElement = MainDeviceProfileNode->ToElement();
							NormalSettingListNode = NormalSettingListElement->FirstChildElement();
							for(;NormalSettingListNode != NULL;NormalSettingListNode = NormalSettingListNode->NextSibling()) {
								////printf("NormalSettingListNode: 0x%x   %s\n",NormalSettingListNode,NormalSettingListNode->Value()); 
								if(!strcmp(NormalSettingListNode->Value(),"PlcMemoryProfileInfo")) {
									PlcMemoryProfileInfoElement = NormalSettingListNode->ToElement();
									PlcMemoryProfileInfoNode = PlcMemoryProfileInfoElement->FirstChildElement();
									for(;PlcMemoryProfileInfoNode != NULL;PlcMemoryProfileInfoNode = PlcMemoryProfileInfoNode->NextSibling()) {
										////printf("PlcMemoryProfileInfoNode: 0x%x   %s\n",PlcMemoryProfileInfoNode,PlcMemoryProfileInfoNode->Value()); 
									}
								}
							}
						} else if(!strcmp(MainDeviceProfileNode->Value(),"DataFormat")){
							machine->mainDeviceProfile.DataFormat = MainDeviceProfileNode->ToElement()->GetText();
						} 
					}
				} else if(!strcmp(LineMachineNode->Value(),"CarrierSlotCount")) {
					string size = LineMachineNode->ToElement()->GetText();
					machine->CarrierSlotCount = strtoul(size.c_str(), NULL, 10);
					
				} else if(!strcmp(LineMachineNode->Value(),"TraySlotCount")) {
					string size = LineMachineNode->ToElement()->GetText();
					machine->TraySlotCount = strtoul(size.c_str(), NULL, 10);

				} else if(!strcmp(LineMachineNode->Value(),"Uph")) {
					string size = LineMachineNode->ToElement()->GetText();
					machine->Uph = strtoul(size.c_str(), NULL, 10);
					
				} else if(!strcmp(LineMachineNode->Value(),"Stations")) {
					StationsElement = LineMachineNode->ToElement();
					StationsNode = StationsElement->FirstChildElement();
					for(;StationsNode != NULL;StationsNode = StationsNode->NextSibling()) {
						////printf("StationsNode: 0x%x   %s\n",StationsNode,StationsNode->Value()); 
						if(!strcmp(StationsNode->Value(),"LineStation")) {
							LineStation_t *ls = new(LineStation_t);
							LineStationElement = StationsNode->ToElement();
							LineStationNode = LineStationElement->FirstChildElement();
							for(;LineStationNode != NULL;LineStationNode = LineStationNode->NextSibling()) {
								////printf("LineStationNode: 0x%x   %s\n",LineStationNode,LineStationNode->Value()); 
								if(!strcmp(LineStationNode->Value(),"Events")) {
									EventsElement = LineStationNode->ToElement();
									EventsNode = EventsElement->FirstChildElement();
									for(;EventsNode != NULL;EventsNode = EventsNode->NextSibling()) {
										////printf("EventsNode--2: 0x%x   %s\n",EventsNode,EventsNode->Value()); 
										if(!strcmp(EventsNode->Value(),"StationEventProfile")) {
											StationEventProfileElement = EventsNode->ToElement();
											StationEventProfileNode = StationEventProfileElement->FirstChildElement();
											StationEventProfile_t *ev_profile = new(StationEventProfile_t);
											for(;StationEventProfileNode != NULL;StationEventProfileNode = StationEventProfileNode->NextSibling()) {
												////printf("StationEventProfileNode---2: 0x%x   %s =====---====\n",StationEventProfileNode,StationEventProfileNode->Value()); 
												// TODO: ======= create station event profile =============
												GetEventProfile(ev_profile,StationEventProfileNode);
											}

											if(!strcmp(ev_profile->Properties.c_str(),"NoPlcEvent")){
												delete ev_profile;
											} else {
												ls->Events.push_back(ev_profile);
											}
										}
									}
								} else if (!strcmp(LineStationNode->Value(),"Name")) {
									ls->Name = LineStationNode->ToElement()->GetText();
								} else if (!strcmp(LineStationNode->Value(),"Enable")) {
									if(!strcmp(LineStationNode->ToElement()->GetText(),"true")){
										ls->Enable = true;
									} else if(!strcmp(LineStationNode->ToElement()->GetText(),"false")){
										ls->Enable = false;
									} else {
										printf("Enable text error: %s\n", LineStationNode->ToElement()->GetText());
									} 
								} else if (!strcmp(LineStationNode->Value(),"UpdateTimeTicks")) {
									string ticks = LineStationNode->ToElement()->GetText();
									ls->UpdateTimeTicks = strtoull (ticks.c_str(), NULL, 0);
								} else if (!strcmp(LineStationNode->Value(),"MaxInputCacheCarrierCount")) {
									string MaxInputCacheCarrierCount = LineStationNode->ToElement()->GetText();
									ls->MaxInputCacheCarrierCount = strtoul(MaxInputCacheCarrierCount.c_str(), NULL, 10);
								} else if (!strcmp(LineStationNode->Value(),"MaxOutputCacheCarrierCount")) {
									string MaxOutputCacheCarrierCount = LineStationNode->ToElement()->GetText();
									ls->MaxOutputCacheCarrierCount = strtoul(MaxOutputCacheCarrierCount.c_str(), NULL, 10);
								} else if (!strcmp(LineStationNode->Value(),"Properties")) {
									ls->Properties = LineStationNode->ToElement()->GetText();
								}		
							}
							machine->Stations.push_back(ls);
						}
					}
				} 
			}
			machine->p_mainEvents = &machine->mainDeviceProfile.mainEvents;
			//machine->p_customEvents = &machine->
			lm->push_back(machine);
		}
	}

	return lm;
}

vector<LineSection_t*> *GetLineSectionList(TiXmlNode *lineSectionNode)
{
	TiXmlNode* lineSectionPropertyNode;
	TiXmlNode* MachineNode;

	TiXmlElement* LineSectionElement;
	TiXmlElement* MachinesElement;

	vector<LineSection_t*>* sectionList = new vector<LineSection_t*>;


	for(;lineSectionNode != NULL;lineSectionNode = lineSectionNode->NextSibling()) {
		LineSection_t* section = new LineSection_t;
		LineSectionElement = lineSectionNode->ToElement();
		lineSectionPropertyNode = LineSectionElement->FirstChildElement();
		for(;lineSectionPropertyNode != NULL;lineSectionPropertyNode = lineSectionPropertyNode->NextSibling()) {
					//printf("lineSectionPropertyNode: 0x%x   %s\n",lineSectionPropertyNode,lineSectionPropertyNode->Value());
			if(!strcmp(lineSectionPropertyNode->Value(),"Machines")) {
				MachinesElement = lineSectionPropertyNode->ToElement();
				MachineNode = MachinesElement->FirstChildElement();
				section->Machines = GetLineMachineList(MachineNode);
			} else if (!strcmp(lineSectionPropertyNode->Value(),"Name")) {
				section->Name = lineSectionPropertyNode->ToElement()->GetText();
			} else if (!strcmp(lineSectionPropertyNode->Value(),"Enable")) {
				string Enable = lineSectionPropertyNode->ToElement()->GetText();
				if(!strcmp(lineSectionPropertyNode->ToElement()->GetText(),"true")){
					section->Enable = true;
				} else if(!strcmp(lineSectionPropertyNode->ToElement()->GetText(),"false")){
					section->Enable = false;
				} else {
					printf("Enable text error: %s\n", lineSectionPropertyNode->ToElement()->GetText());
				}
			} else if (!strcmp(lineSectionPropertyNode->Value(),"UpdateTimeTicks")) {
				string UpdateTimeTicks = lineSectionPropertyNode->ToElement()->GetText();
				section->UpdateTimeTicks = strtoull (UpdateTimeTicks.c_str(), NULL, 0);
			} else if (!strcmp(lineSectionPropertyNode->Value(),"SectionID")) {
				section->SectionID = lineSectionPropertyNode->ToElement()->GetText();
			} else if (!strcmp(lineSectionPropertyNode->Value(),"ReadOnly")) {
				string ReadOnly = lineSectionPropertyNode->ToElement()->GetText();
				if(!strcmp(lineSectionPropertyNode->ToElement()->GetText(),"true")){
					section->ReadOnly = true;
				} else if(!strcmp(lineSectionPropertyNode->ToElement()->GetText(),"false")){
					section->ReadOnly = false;
				} else {
					printf("ReadOnly text error: %s\n", lineSectionPropertyNode->ToElement()->GetText());
				}
			}
		}

		sectionList->push_back(section);
	}

	return sectionList;
}

LineStationDesiginProfile_t* GetLineStationDesiginProfile(string xmlFile)
{
	LineStationDesiginProfile_t* lsdp = new LineStationDesiginProfile_t;

	TiXmlDocument doc(xmlFile.c_str() );
	bool loadOkay = doc.LoadFile();

	if ( !loadOkay ){
		printf( "Could not load test file 'demotest.xml'..\n" );
		delete lsdp;
		return NULL;
	}


	TiXmlPrinter printer;
	doc.Accept( &printer );

	TiXmlNode* node = 0;
	TiXmlNode* lineSectionNode = NULL;

	TiXmlElement* LineSectionsElement = NULL;



	node = doc.RootElement();
	assert( node );

	node = node->FirstChild();
	for(; node != NULL; node = node->NextSibling()) {
		
		if (!strcmp(node->Value(),"LineSections")) {
			LineSectionsElement = node->ToElement();
			lineSectionNode = LineSectionsElement->FirstChildElement();
			lsdp->LineSections = GetLineSectionList(lineSectionNode);
		} else if (!strcmp(node->Value(),"Assembly")) {
			lsdp->Assembly = node->ToElement()->GetText();
			
		} else if (!strcmp(node->Value(),"PluginName")) {
			lsdp->PluginName = node->ToElement()->GetText();
			
		} else if (!strcmp(node->Value(),"Version")) {
			lsdp->Version = node->ToElement()->GetText();
			
		} else if (!strcmp(node->Value(),"Enable")) {
			string Enable = node->ToElement()->GetText();
			
			if(!strcmp(node->ToElement()->GetText(),"true")){
				lsdp->Enable = true;
			} else if(!strcmp(node->ToElement()->GetText(),"false")){
				lsdp->Enable = false;
			} else {
				printf("Enable text error: %s\n", node->ToElement()->GetText());
			}
			
		} else if (!strcmp(node->Value(),"AutoLoaded")) {
			string AutoLoaded = node->ToElement()->GetText();
			if(!strcmp(node->ToElement()->GetText(),"true")){
				lsdp->AutoLoaded = true;
			} else if(!strcmp(node->ToElement()->GetText(),"false")){
				lsdp->AutoLoaded = false;
			} else {
				printf("Enable text error: %s\n", node->ToElement()->GetText());
			}
			
		} else if (!strcmp(node->Value(),"DefaultCarrierSlotCount")) {
			string DefaultCarrierSlotCount = node->ToElement()->GetText();
			lsdp->DefaultCarrierSlotCount = strtoul(DefaultCarrierSlotCount.c_str(), NULL, 10);
		} else if (!strcmp(node->Value(),"DefaultTraySlotCount")) {
			string DefaultTraySlotCount = node->ToElement()->GetText();
			lsdp->DefaultTraySlotCount = strtoul(DefaultTraySlotCount.c_str(), NULL, 10);
		}
	}

	return lsdp;
}