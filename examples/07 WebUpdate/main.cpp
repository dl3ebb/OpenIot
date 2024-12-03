/**
 * Demo of the Web Update Server
 * 
 * To use this example you must 
 *   1. set up a web update server 
 *   2. Deploy a version of your application to this server. 
 * 
 */

#define SWITCH_PIN 25
#define LED_PIN    32
#define WLAN_SSID   "your_ssid"
#define WLAN_PASSWD "your_passwd"
#define WEB_UPDATE_SERVER "your_update_server"

#include <OpenIot.h>
#include <Connector/DigitalInConnector.h>
#include <Connector/DigitalOutConnector.h>
#include <Logger/SerialLogger.h>
#include <Modules/WebUpdate.h>

#include <Modules/Wifi.h>

void setup() {
    serialLogger->setBaudrate(115200);
    openIot.setLogger(serialLogger);
    
    wifi->setEnablePrimary(true);
    wifi->setSsid1(WLAN_SSID);
    wifi->setPassword1(WLAN_PASSWD);

    webUpdate->setEnable(true);
    webUpdate->setServer(WEB_UPDATE_SERVER);
    webUpdate->setAutoUpdate(true);

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

    openIot.setup("OpenIotWebUpdate", 1);
}

void loop() {
    openIot.loop();
}