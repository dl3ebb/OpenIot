/**
 * Demo of the REST Server usage
 */
#define SWITCH_PIN 25
#define LED_PIN    32
#define WLAN_SSID   "your_ssid"
#define WLAN_PASSWD "your_passwd"
#include <OpenIot.h>
#include <Connector/DigitalInConnector.h>
#include <Connector/DigitalOutConnector.h>
#include <Connector/RestConnector.h>
#include <Logger/SerialLogger.h>

#include <Modules/Wifi.h>

void setup() {
    serialLogger->setBaudrate(115200);
    openIot.setLogger(serialLogger);
    
    wifi->setEnablePrimary(true);
    wifi->setSsid1(WLAN_SSID);
    wifi->setPassword1(WLAN_PASSWD);

    restConnector->setEnable(true);

    /**
     * Create the Elements and register them to the connectors
     */
    BoolElement *switch1 = new BoolElement("Switch1");
    digitalInConnector->registerElement(switch1, SWITCH_PIN, true);
    restConnector->registerElement(switch1, "Switch1");

    BoolElement *led1 = new BoolElement("Led1");
    digitalOutConnector->registerElement(led1, LED_PIN);
    restConnector->registerElement(led1, "Led1");
    
    eventManager.addListener<NewBoolValueEvent>(NewBoolValue, "Switch1", [led1](NewBoolValueEvent *event) {
        led1->setValue(event->newValue);
    });

    openIot.setup("OpenIotRest", 1);
}

void loop() {
    openIot.loop();
}