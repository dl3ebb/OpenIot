/**
 * Demo of the web frontend
 * 
 * Dont forget to copy the web pages from the library and from this example to your 
 * project's data directory and create and umload the fileimage to the ESP Chip. 
 * 
 */

#define SWITCH_PIN 25
#define LED_PIN    32
#define WLAN_SSID   "your_ssid"
#define WLAN_PASSWD "your_passwd"

#include <OpenIot.h>
#include <Connector/DigitalInConnector.h>
#include <Connector/DigitalOutConnector.h>
#include <Connector/WebConnector.h>
#include <Logger/SerialLogger.h>
#include <Modules/Wifi.h>

#include <Webpages/IndexPage.h>
#include <Webpages/DeviceForm.h>
#include <Webpages/HotspotForm.h>
#include <Webpages/LoggingForm.h>
#include <Webpages/MqttForm.h>
#include <Webpages/NtpForm.h>
#include <Webpages/RebootForm.h>
#include <Webpages/WebUpdateForm.h>
#include <Webpages/WifiForm.h>

void setup() {
    serialLogger->setBaudrate(115200);
    openIot.setLogger(serialLogger);

    //--- Register all Configpages -----
    openIot.registerForm(indexPage);
    openIot.registerForm(hotspotForm);
    openIot.registerForm(wifiForm);
    openIot.registerForm(mqttForm);
    openIot.registerForm(ntpForm);
    openIot.registerForm(deviceForm);
    openIot.registerForm(loggingForm);
    openIot.registerForm(webUpdateForm);
    openIot.registerForm(rebootForm);

    /**
     * Create the Elements and register them to the connectors
     */
    BoolElement *switch1 = new BoolElement("Switch1");
    digitalInConnector->registerElement(switch1, SWITCH_PIN, true);
    webConnector->registerElement(switch1);

    BoolElement *led1 = new BoolElement("Led1");
    digitalOutConnector->registerElement(led1, LED_PIN);
    webConnector->registerElement(led1);

    eventManager.addListener<NewBoolValueEvent>(NewBoolValue, "Switch1", [led1](NewBoolValueEvent *event) {
        led1->setValue(event->newValue);
    });

    openIot.setup("OpenIotSwitch", 1);
}

void loop() {
    openIot.loop();
}