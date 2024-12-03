/**
 * This is a Hello World example for OpenIot. 
 * 
 * To run this example connect a led to a I/O Pin and a pushbutton to another I/O Pin. The 
 * pushbutton should switch to ground and have a pullup resistor to vcc
 */

#define SWITCH_PIN 25
#define LED_PIN    32

#include <OpenIot.h>
#include <Connector/DigitalInConnector.h>
#include <Connector/DigitalOutConnector.h>


void setup() {
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

    openIot.setup("HelloOpenIot", 1;
}

void loop() {
    openIot.loop();
}