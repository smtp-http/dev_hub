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
	}else if(!strcmp(profileElement->Value(),"PlcBlockAddress")) {
		string addr = profileElement->GetText();
		event->PlcBlockAddress = strtoul(addr.c_str(), NULL, 10);
		
	}else if(!strcmp(profileElement->Value(),"PlcBlockSize")) {
		string size = profileElement->GetText();
		event->PlcBlockSize = strtoul(size.c_str(), NULL, 10);
	}else if(!strcmp(profileElement->Value(),"EapBlockAddress")) {
		string addr = profileElement->GetText();
		event->EapBlockAddress = strtoul(addr.c_str(), NULL, 10);
		//cout << "**********---------" << profileElement->Value() << ": " << event->EapBlockAddress << endl;
	}else if(!strcmp(profileElement->Value(),"EapBlockSize")) {
		string addr = profileElement->GetText();
		event->EapBlockSize = strtoul(addr.c_str(), NULL, 10);
	} else{
		return -1;
	}

	return 0;
}

vector<StationEventProfile_t*> *GetAllStationEventProfile(string xmlFile)
{
	vector<StationEventProfile_t*> *vs = new(vector<StationEventProfile_t*>);

	TiXmlDocument doc(xmlFile.c_str() );
	bool loadOkay = doc.LoadFile();

	if ( !loadOkay ){
		//printf( "Could not load test file 'demotest.xml'. Error='%s'. Exiting.\n", doc.ErrorDesc() );
		delete vs;
		return NULL;
	}

	//printf( "** Printing via TiXmlPrinter **\n" );
	TiXmlPrinter printer;
	doc.Accept( &printer );
			//f//printf( stdout, "%s", printer.CStr() );


	TiXmlNode* node = 0;
	TiXmlNode* lineSectionNode = NULL;
	TiXmlNode* lineSectionPropertyNode = NULL;
	TiXmlNode* MachineNode = NULL;
	TiXmlNode* LineMachineNode = NULL;
	TiXmlNode* MainDeviceProfileNode = NULL;

	TiXmlNode* NormalSettingsNode = NULL;
	TiXmlNode* MotorSettingsNode = NULL;
	TiXmlNode* PlcDataBlockNode = NULL;
	TiXmlNode* EapDataBlockNode = NULL;
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
	TiXmlElement* NormalSettingsElement = NULL;
	TiXmlElement* MotorSettingsElement = NULL;
	TiXmlElement* PlcDataBlockElement = NULL;
	TiXmlElement* EapDataBlockElement = NULL;
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
								LineMachineElement = MachineNode->ToElement();
								LineMachineNode = LineMachineElement->FirstChildElement();
								for(;LineMachineNode != NULL;LineMachineNode = LineMachineNode->NextSibling()) {
									//printf("LineMachineNode: 0x%x   %s\n",LineMachineNode,LineMachineNode->Value()); 
									if(!strcmp(LineMachineNode->Value(),"MainDeviceProfile")) {
										MainDeviceProfileElement = LineMachineNode->ToElement();
										MainDeviceProfileNode = MainDeviceProfileElement->FirstChildElement();
										for(;MainDeviceProfileNode != NULL;MainDeviceProfileNode = MainDeviceProfileNode->NextSibling()) {
											//printf("MainDeviceProfileNode: 0x%x   %s\n",MainDeviceProfileNode,MainDeviceProfileNode->Value()); 
											if(!strcmp(MainDeviceProfileNode->Value(),"NormalSettings")) {
												NormalSettingsElement = MainDeviceProfileNode->ToElement();
												NormalSettingsNode = NormalSettingsElement->FirstChildElement();
												for(;NormalSettingsNode != NULL;NormalSettingsNode = NormalSettingsNode->NextSibling()) {
													//printf("NormalSettingsNode: 0x%x   %s\n",NormalSettingsNode,NormalSettingsNode->Value()); 
												}
											}
											if(!strcmp(MainDeviceProfileNode->Value(),"MotorSettings")) {
												MotorSettingsElement = MainDeviceProfileNode->ToElement();
												MotorSettingsNode = MotorSettingsElement->FirstChildElement();
												for(;MotorSettingsNode != NULL;MotorSettingsNode = MotorSettingsNode->NextSibling()) {
													//printf("MotorSettingsNode: 0x%x   %s\n",MotorSettingsNode,MotorSettingsNode->Value()); 
												}
											}
											if(!strcmp(MainDeviceProfileNode->Value(),"PlcDataBlock")) {
												PlcDataBlockElement = MainDeviceProfileNode->ToElement();
												PlcDataBlockNode = PlcDataBlockElement->FirstChildElement();
												for(;PlcDataBlockNode != NULL;PlcDataBlockNode = PlcDataBlockNode->NextSibling()) {
													//printf("PlcDataBlockNode: 0x%x   %s\n",PlcDataBlockNode,PlcDataBlockNode->Value()); 
												}
											}
											if(!strcmp(MainDeviceProfileNode->Value(),"EapDataBlock")) {
												EapDataBlockElement = MainDeviceProfileNode->ToElement();
												EapDataBlockNode = EapDataBlockElement->FirstChildElement();
												for(;EapDataBlockNode != NULL;EapDataBlockNode = EapDataBlockNode->NextSibling()) {
													//printf("EapDataBlockNode: 0x%x   %s\n",EapDataBlockNode,EapDataBlockNode->Value()); 
												}
											}
											if(!strcmp(MainDeviceProfileNode->Value(),"Events")) {
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
														vs->push_back(ev_profile);
													}
												}
											}
											if(!strcmp(MainDeviceProfileNode->Value(),"AlarmCodeList")){
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
											}
											if(!strcmp(MainDeviceProfileNode->Value(),"NormalSettingList")){
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
									}

									if(!strcmp(LineMachineNode->Value(),"Stations")) {
										StationsElement = LineMachineNode->ToElement();
										StationsNode = StationsElement->FirstChildElement();
										for(;StationsNode != NULL;StationsNode = StationsNode->NextSibling()) {
											////printf("StationsNode: 0x%x   %s\n",StationsNode,StationsNode->Value()); 
											if(!strcmp(StationsNode->Value(),"LineStation")) {
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
																vs->push_back(ev_profile);
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}


	delete vs;
	return NULL;
}
