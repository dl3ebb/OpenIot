#include "DigitalInConnector.h"
#include <Core/EventManager.h>
#include <OpenIot.h>

DigitalInConnector::DigitalInConnector() : Connector() {
    openIot.registerConnector(this);
}

void DigitalInConnector::registerElement(BoolElement *element_, int pin_, bool invert_) {
    DigitalInConnectorElement *element = new DigitalInConnectorElement(element_->id, pin_, invert_);
    pinMode(element->pin, INPUT);
    bool rd = (digitalRead(element->pin) == HIGH);
    if (element->invert) {
        rd = !rd;
    }
    element->value = rd;
    elements.push_back(element);
}

void DigitalInConnector::setup() {};

void DigitalInConnector::loop(bool firstLoop) {

    for (DigitalInConnectorElement *element : elements) {
        bool rd = (digitalRead(element->pin) == HIGH);
        if (element->invert) {
            rd = !rd;
        }
        if (element->value != rd || firstLoop) {
            element->value = rd;
            eventManager.fireEvent(new NewBoolValueEvent(element->id, rd));
        }
    }
};

DigitalInConnector *_digitalInConnector = nullptr;

DigitalInConnector *getDigitalInConnector() {
    if (!_digitalInConnector) {
        _digitalInConnector = new DigitalInConnector();
    }
    return _digitalInConnector;
}
