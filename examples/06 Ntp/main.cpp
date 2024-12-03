/**
 * Demo of the NTP Server usage
 */

#define SWITCH_PIN 25
#define LED_PIN    32
#define WLAN_SSID   "your_ssid"
#define WLAN_PASSWD "your_passwd"

#include <OpenIot.h>
#include <Connector/DigitalInConnector.h>
#include <Connector/DigitalOutConnector.h>
#include <Modules/Ntp.h>
#include <Logger/SerialLogger.h>

#include <Modules/Wifi.h>

void setup() {
    serialLogger->setBaudrate(115200);
    openIot.setLogger(serialLogger);
    
    wifi->setEnablePrimary(true);
    wifi->setSsid1(WLAN_SSID);
    wifi->setPassword1(WLAN_PASSWD);

    ntp->setEnable(true);

    /**
     * Create the Elements and register them to the connectors
     */
    BoolElement *switch1 = new BoolElement("Switch1");
    digitalInConnector->registerElement(switch1, SWITCH_PIN, true);

    BoolElement *led1 = new BoolElement("Led1");
    digitalOutConnector->registerElement(led1, LED_PIN);
    
    eventManager.addListener<NewBoolValueEvent>(NewBoolValue, "Switch1", [led1](NewBoolValueEvent *event) {
        led1->setValue(event->newValue);
    });

    openIot.setup("OpenIotNtp", 1);
}

void loop() {
    openIot.loop();
}