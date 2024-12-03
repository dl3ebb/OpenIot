
#ifndef MQTT_STATUS_H
#define MQTT_STATUS_H
#include "Module.h"
#include "time.h"
#include <Core/Element.h>

/**
 * The MQTT Status Module sends some internal properties like speed, memory usage etc. to an external mqtt server. 
 */
class MqttStatus : public Module {
  private:
    long loopCounter = 0;
    long upTimeSeconds = 0;
    struct tm timeinfo;
    char timebuffer[32];
    char uptimebuffer[32];
    char ipBuffer[20];

    LongElement   *loopCounterElement;
    StringElement *localTimeElement;
    StringElement *upTimeElement;
    StringElement *ipAddressElement;
    StringElement *wifiSsidElement;
    LongElement   *wifiRssiElement;
    LongElement   *heapSizeElement;

  public:
    MqttStatus();
    void setup();
    void loop(bool firstLoop);
    void updateStatus();
    void enable () {}
};

#define mqttStatus getMqttStatus()

MqttStatus *getMqttStatus();

#endif
