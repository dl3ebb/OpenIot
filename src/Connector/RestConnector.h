#ifndef REST_CONNECTOR_H
#define REST_CONNECTOR_H
#include "Connector.h"
#include <Arduino.h>
#include <Core/Config/Config.h>
#include <Core/Element.h>
#include <ESPAsyncWebServer.h>

/**
 * RestConnectorElement contains the data to connect an Element to the REST service
 */
class RestConnectorElement {
  public:
    String topic;
    Element *element;

    bool boolValue;
    long longValue;
    String stringValue;

    RestConnectorElement(Element *element_, String topic_) : element(element_) {
        topic = topic_;
    }
};

class RestConnector : public Connector {
  private:
    ConfigBool *enable = new ConfigBool("OI/Rest", "enable", false, true);

    std::map<String, RestConnectorElement *> restElements;

  public:
    RestConnector();

    void setup() override;
    void loop(bool firstLoop) override {}

    void registerElement(Element *element_, String topic);

    //--- Configuration ----------------------------------
    void setEnable(bool enable_) { enable->setValue(enable_); }

    bool getEnable() { return enable->getValue(); }

  private:
    void logSettings();
};

#define restConnector getRestConnector()

RestConnector *getRestConnector();

#endif