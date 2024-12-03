/**
 * Demo of the MQTT Usage
 */

#define SWITCH_PIN 25
#define LED_PIN    32
#define WLAN_SSID   "your_ssid"
#define WLAN_PASSWD "your_passwd"
#define MQTT_SERVER "your.mqtt.server"
#define MQTT_PREFIX "/Labs/HelloMqqt"


#include <OpenIot.h>
#include <Connector/DigitalInConnector.h>
#include <Connector/DigitalOutConnector.h>
#include <Connector/MqttConnector.h>
#include <Logger/SerialLogger.h>

#include <Modules/Wifi.h>

void setup() {
    serialLogger->setBaudrate(115200);
    openIot.setLogger(serialLogger);
    
    wifi->setEnablePrimary(true);
    wifi->setSsid1(WLAN_SSID);
    wifi->setPassword1(WLAN_PASSWD);

    mqttConnector->setEnable(true);
    mqttConnector->setServer (MQTT_SERVER);
    mqttConnector->setPrefix (MQTT_PREFIX);

    /**
     * Create the Elements and register them to the connectors
     */
    BoolElement *switch1 = new BoolElement("Switch1");
    digitalInConnector->registerElement(switch1, SWITCH_PIN, true);
    mqttConnector->registerPublish(switch1, "/app/Switch1/status");

    BoolElement *led1 = new BoolElement("Led1");
    digitalOutConnector->registerElement(led1, LED_PIN);
    mqttConnector->registerSubscribe(led1, "/app/Led1/command");
    mqttConnector->registerPublish(led1, "/app/Led1/status");

    eventManager.addListener<NewBoolValueEvent>(NewBoolValue, "Switch1", [led1](NewBoolValueEvent *event) {
        led1->setValue(event->newValue);
    });

    openIot.setup("OpenIotMqtt", 1);
}

void loop() {
    openIot.loop();
}