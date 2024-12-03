#ifndef DIGITAL_IN_CONNECTOR_H
#define DIGITAL_IN_CONNECTOR_H

#include "Connector.h"
#include <Core/Element.h>
#include <stdint.h>
#include <vector>

/**
 * DigitalInConnectorElement is the container for one connection between a BoolElement and a digital I/O pin
 */
class DigitalInConnectorElement {
  public:
    bool value;
    String id;
    uint8_t pin;
    bool invert;

    DigitalInConnectorElement(String id_, int pin_, bool invert_ = false) : value(false), id(id_), pin(pin_), invert(invert_) {}
};

/**
 * DigitalInConnector connects BoolElements to digital I/O bins
 */
class DigitalInConnector : public Connector {
  public:
    DigitalInConnector();
    void setup() override;
    void loop(bool firstLoop) override;
    void registerElement(BoolElement *element, int pin_, bool invert_);

    std::vector<DigitalInConnectorElement *> elements;
};

#define digitalInConnector getDigitalInConnector()

DigitalInConnector *getDigitalInConnector();

#endif