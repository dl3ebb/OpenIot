#ifndef CONNECTOR_H
#define CONNECTOR_H

/**
 * Connector ist the virtual base class for all Connectors.
 */
class Connector {
  public:
    virtual ~Connector() = default;

    virtual void setup() = 0;
    virtual void loop(bool firstLoop) = 0;
    void depend() {}
};

#endif