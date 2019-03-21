#ifndef __MQTT_H__
#define __MQTT_H__

#include <vector>
#include <string>
#include "connbase.h"
#include "eventlooper.h"

class Mqtt : public lux::IConnectorAcceptorSink,public lux::IUpdateSink,public lux::ITransUpdate
{

public:
	static int mqtt();

};


#endif

