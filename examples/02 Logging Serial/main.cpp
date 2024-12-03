/**
 * Demo of Serial Logging
 */

#define SWITCH_PIN 25
#define LED_PIN    32

#include <OpenIot.h>
#include <Connector/DigitalInConnector.h>
#include <Connector/DigitalOutConnector.h>
#include <Logger/SerialLogger.h>

void setup() {
    serialLogger->setBaudrate(115200);
    openIot.setLogger(serialLogger);
    
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

    openIot.setup("OpenIotLogging", 1);
}

void loop() {
    openIot.loop();
}