#include "DigitalOutConnector.h"
#include <OpenIot.h>
#include <Core/EventManager.h>
#include <Core/Element.h>


DigitalOutConnector::DigitalOutConnector() : Connector() {
    openIot.registerConnector(this);
    eventManager.addListener<NewBoolValueEvent>(NewBoolValue, [this](NewBoolValueEvent* event) {
        if (this->elements.find(event->destination) != this->elements.end()) {
            DigitalOutConnectorElement* ele = this->elements[event->destination];
            digitalWrite(ele->pin, event->newValue);
        }
    });
}

void DigitalOutConnector::registerElement (Element *element_, int pin_, bool invert_) {
    DigitalOutConnectorElement* element = new DigitalOutConnectorElement (element_, pin_, invert_);
    pinMode(element->pin, OUTPUT);
    elements[element->element->id] = element;
}


DigitalOutConnector* _digitalOutConnector = nullptr; 

DigitalOutConnector* getDigitalOutConnector() {
    if (!_digitalOutConnector) {
        _digitalOutConnector = new DigitalOutConnector();
    }
    return _digitalOutConnector;
}


