#ifndef DIGITAL_OUT_CONNECTOR_H
#define DIGITAL_OUT_CONNECTOR_H
#include "Connector.h"
#include <Core/Element.h>
#include <stdint.h>
#include <vector>

/**
 * DigitalOutConnectorElement is the container for one connection between a BoolElement and a digital I/O pin
 */
class DigitalOutConnectorElement {
  public:
    Element *element;
    uint8_t pin;
    bool invert;

    DigitalOutConnectorElement(Element *element_, int pin_, bool invert_) : element(element_), pin(pin_), invert(invert_) {}
    DigitalOutConnectorElement(Element *element_, int pin_) : element(element_), pin(pin_), invert(false) {}
};

/**
 * DigitalInConnector connects BoolElements to digital I/O bins
 */
class DigitalOutConnector : public Connector {
  public:
    DigitalOutConnector();
    void setup() override {}
    void loop(bool firstLoop) override {};
    void registerElement(Element *element_, int pin_, bool invert_ = false);

    std::map<String, DigitalOutConnectorElement *> elements;
};

#define digitalOutConnector getDigitalOutConnector()

DigitalOutConnector *getDigitalOutConnector();

#endif