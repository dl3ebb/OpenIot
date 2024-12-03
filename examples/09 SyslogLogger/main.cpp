/**
 * Demo of the Syslog Logger 
 */

#define SWITCH_PIN 25
#define LED_PIN    32
#define WLAN_SSID   "your_ssid"
#define WLAN_PASSWD "your_passwd"
#define SYSLOG_SERVER "your.syslog.server"

#include <OpenIot.h>
#include <Connector/DigitalInConnector.h>
#include <Connector/DigitalOutConnector.h>
#include <Logger/SerialLogger.h>
#include <Logger/SyslogLogger.h>

#include <Modules/Wifi.h>

void setup() {
    serialLogger->setBaudrate(115200);

    syslogLogger->setEnable(true);
    syslogLogger->setLogServer(SYSLOG_SERVER);
    syslogLogger->setBackupLogger(serialLogger);
    openIot.setLogger(syslogLogger);
    
    wifi->setEnablePrimary(true);
    wifi->setSsid1(WLAN_SSID);
    wifi->setPassword1(WLAN_PASSWD);

    /**
     * Create the Elements and register them to the connectors
     */
    BoolElement *switch1 = new BoolElement("Switch1");
    digitalInConnector->registerElement(switch1, SWITCH_PIN, true);

    BoolElement *led1 = new BoolElement("Led1");
    digitalOutConnector->registerElement(led1, LED_PIN);

    eventManager.addListener<NewBoolValueEvent>(NewBoolValue, "Switch1", [led1](NewBoolValueEvent *event) {
        led1->setValue(event->newValue);
        logger->info (F("LED new Value : %s"), event->newValue ? "true" : "false");
    });

    openIot.setup("OpenIotSysLog", 1);
}

void loop() {
    openIot.loop();
}