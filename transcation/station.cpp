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
		string addr = profileElement->GetText();
		event->PlcBlockAddress = strtoul(addr.c_str(), NULL, 10);
	}else if(!strcmp(profileElement->Value(),"PlcBlockSize")) {
		string size = profileElement->GetText();
		event->PlcBlockSize = strtoul(size.c_str(), NULL, 10);
	}else if(!strcmp(profileElement->Value(),"EapBlockAddress")) {
		string addr = profileElement->GetText();
		event->EapBlockAddress = strtoul(addr.c_str(), NULL, 10);
	}else if(!strcmp(profileElement->Value(),"EapBlockSize")) {
		string addr = profileElement->GetText();
		event->EapBlockSize = strtoul(addr.c_str(), NULL, 10);
	} else{
		return -1;
	}

	return 0;
}




vector<LineMachine_t*> *GetLineMachineList(string xmlFile)
{
	//vector<StationEventProfile_t*> *vs = new(vector<StationEventProfile_t*>);
	vector<LineMachine_t*>  *lm = new(vector<LineMachine_t*>);

	TiXmlDocument doc(xmlFile.c_str() );
	bool loadOkay = doc.LoadFile();

	if ( !loadOkay ){
		printf( "Could not load test file 'demotest.xml'..\n" );
		delete lm;
		return NULL;
	}


	TiXmlPrinter printer;
	doc.Accept( &printer );



	TiXmlNode* node = 0;
	TiXmlNode* lineSectionNode = NULL;
	TiXmlNode* lineSectionPropertyNode = NULL;
	TiXmlNode* MachineNode = NULL;
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



	TiXmlElement* LineSectionsElement = NULL;
	TiXmlElement* LineSectionElement = NULL;
	TiXmlElement* MachinesElement = NULL;
	TiXmlElement* LineMachineElement = NULL;
	TiXmlElement* MainDeviceProfileElement = NULL;
	TiXmlElement* todoElement = 0;
	TiXmlElement* itemElement = 0;
	TiXmlElement* SettingsElement = NULL;

	TiXmlElement* EventsElement = NULL;
	TiXmlElement* AlarmCodeListElement = NULL;
	TiXmlElement* StationEventProfileElement = NULL;
	TiXmlElement* PlcAlarmInfoElement = NULL;
	TiXmlElement* NormalSettingListElement = NULL;
	TiXmlElement* PlcMemoryProfileInfoElement = NULL;
	TiXmlElement* StationsElement = NULL;
	TiXmlElement* LineStationElement = NULL;



	node = doc.RootElement();
	assert( node );

	node = node->FirstChild();
	for(; node != NULL; node = node->NextSibling()) {
		//printf("node: 0x%x   %s\n",node,node->Value());
		if(!strcmp(node->Value(),"LineSections")) {
			LineSectionsElement = node->ToElement();
			lineSectionNode = LineSectionsElement->FirstChildElement();
			for(;lineSectionNode != NULL;lineSectionNode = lineSectionNode->NextSibling()) {
				//printf("lineSectionNode: 0x%x   %s\n",lineSectionNode,lineSectionNode->Value());
				LineSectionElement = lineSectionNode->ToElement();
				lineSectionPropertyNode = LineSectionElement->FirstChildElement();
				for(;lineSectionPropertyNode != NULL;lineSectionPropertyNode = lineSectionPropertyNode->NextSibling()) {
					//printf("lineSectionPropertyNode: 0x%x   %s\n",lineSectionPropertyNode,lineSectionPropertyNode->Value());
					if(!strcmp(lineSectionPropertyNode->Value(),"Machines")) {
						MachinesElement = lineSectionPropertyNode->ToElement();
						MachineNode = MachinesElement->FirstChildElement();
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
														
													}
												}

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
														for(;PlcAlarmInfoNode != NULL;PlcAlarmInfoNode = PlcAlarmInfoNode->NextSibling()) {
															////printf("PlcAlarmInfoNode: 0x%x   %s\n",PlcAlarmInfoNode,PlcAlarmInfoNode->Value()); 
														}
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
					}
				}
			}
		}
	}

	return lm;
}

vector<LineStation_t*> *GetLineSectionList(TiXmlNode *node)
{
	vector<LineStation_t*>* sectionList = new vector<LineStation_t*>;



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

	return lsdp;
}