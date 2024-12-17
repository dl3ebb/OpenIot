#ifndef MQTT_CONNECTOR_H
#define MQTT_CONNECTOR_H
#include "Connector.h"
#include <Core/Config/Config.h>
#include <Core/Element.h>

/**
 * MqttConnectorPublishElement contains the data to connect an Element to MQTT for publish
 */
class MqttConnectorPublishElement {
  public:
    String topic;
    Element *element;

    bool boolValue;
    long longValue;
    String stringValue;

    MqttConnectorPublishElement(Element *element_, String topic_) : element(element_), topic(topic_) {}
};

/**
 * MqttConnectorPublishElement contains the data to connect an Element to MQTT for subscribe
 */
class MqttConnectorSubscribeElement {
  public:
    Element *element;

    bool boolValue;
    long longValue;

    MqttConnectorSubscribeElement(Element *element_) : element(element_) {}
};

/**
 * The MQTT Connector connects Element to an MQTT Server
 */
class MqttConnector : public Connector {
  private:
    //--- Configuration Parameters ------------------------------------
    ConfigBool *enable = new ConfigBool("OI/Mqtt", "enable", false, true);
    ConfigString *server = new ConfigString("OI/Mqtt", "server", "", true);
    ConfigLong *port = new ConfigLong("OI/Mqtt", "port", 1883, true);
    ConfigString *prefix = new ConfigString("OI/Mqtt", "prefix", "", true);
    ConfigString *clientId = new ConfigString("OI/Mqtt", "clientId", "", true);
    ConfigString *username = new ConfigString("OI/Mqtt", "username", "", true);
    ConfigString *password = new ConfigString("OI/Mqtt", "password", "", true);

    //--- The Lists of elements for publis / subscribe ---------------------
    std::map<String, MqttConnectorPublishElement *> publishElements;
    std::map<String, MqttConnectorSubscribeElement *> subscribeElements;

  public:
    MqttConnector();

    void setup() override;
    void loop(bool firstLoop) override;

    void registerSubscribe(Element *element_, String topic);
    void registerPublish(Element *element_, String topic);
    void registerGlobalSubscribe(Element *element_, String topic);
    void registerGlobalPublish(Element *element_, String topic);

    //--- Configuration ----------------------------------
    void setEnable(bool enable_) { enable->setValue(enable_); }
    void setServer(const char *server_) { server->setValue(server_); }
    void setPort(long port_) { port->setValue(port_); }
    void setPrefix(const char *prefix_) { prefix->setValue(prefix_); }
    void setClientId(const char *clientId_) { clientId->setValue(clientId_); }
    void setUsername(const char *userName_) { username->setValue(userName_); }
    void setPassword(const char *password_) { password->setValue(password_); }

    bool getEnable() { return enable->getValue(); }
    String getServer() { return server->getValue(); }
    long getPort() { return port->getValue(); }
    String getPrefix() { return prefix->getValue(); }
    String getClientId() { return clientId->getValue(); }
    String getUsername() { return username->getValue(); }
    String getPassword() { return password->getValue(); }

    //--- Eventhandler --------------------------------------
    void onMqttMessage(char *topic, byte *payload, unsigned int length);
    void onMqttConnected();

  private:
    void logSettings();
};

#define mqttConnector getMqttConnector()

MqttConnector *getMqttConnector();

class MqttConnectedEvent : public Event {
  public:
    MqttConnectedEvent() : Event(MqttConnected, "") {}
    void cleanup() override { delete this; }
    String toString() override { return "MqttConnectedEvent"; }
};

#endif