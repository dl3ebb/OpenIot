#include "MqttConnector.h"
#include <Core/EventManager.h>
#include <Modules/Wifi.h>
#include <OpenIot.h>
#include <PubSubClient.h>
#include <WiFi.h>

WiFiClient espClient;
PubSubClient mqttClient(espClient);

MqttConnector::MqttConnector() : Connector() {
    wifi->depend();
    openIot.registerConnector(this);
    eventManager.addListener<MqttConnectedEvent>(MqttConnected, [this](MqttConnectedEvent *event) {
        this->onMqttConnected();
    });

    eventManager.addListener<NewBoolValueEvent>(NewBoolValue, [this](NewBoolValueEvent *event) {
        if (publishElements.find(event->destination) != publishElements.end()) {
            MqttConnectorPublishElement* ele = publishElements[event->destination];
            if (ele->element->getType() == BOOL_TYPE) {
                ele->boolValue = event->newValue;
                mqttClient.publish(ele->topic.c_str(),(ele->boolValue ? "true" : "false"),true);            
            } else {
                logger->warn (F("Invalid type for MQTT publish Element %s : %d expected MQTT_BOOL (%d)"), event->destination.c_str(), ele->element->getType(), BOOL_TYPE);
            }
        } });

    eventManager.addListener<NewLongValueEvent>(NewLongValue, [this](NewLongValueEvent *event) {
        if (publishElements.find(event->destination) != publishElements.end()) {
            MqttConnectorPublishElement* ele = publishElements[event->destination];
            if (ele->element->getType() == LONG_TYPE) {
                ele->longValue = event->newValue;
                mqttClient.publish(ele->topic.c_str(),String(ele->longValue).c_str(),true);            
            } else {
                logger->warn (F("Invalid type for MQTT publish Element %s : %d expected LONG_TYPE (%d)"), event->destination.c_str(), ele->element->getType(), LONG_TYPE);
            }
        } });

    eventManager.addListener<NewStringValueEvent>(NewStringValue, [this](NewStringValueEvent *event) {
        if (publishElements.find(event->destination) != publishElements.end()) {
            MqttConnectorPublishElement* ele = publishElements[event->destination];
            if (ele->element->getType() == STRING_TYPE) {
                ele->stringValue = event->newValue;
                mqttClient.publish(ele->topic.c_str(),ele->stringValue.c_str(),true);            
            } else {
                logger->warn (F("Invalid type for MQTT publish Element %s : %d expected MQTT_STRING (%d)"), event->destination.c_str(), ele->element->getType(), STRING_TYPE);
            }
        } });
}

void callback(char *topic, byte *payload, unsigned int length) {
    mqttConnector->onMqttMessage(topic, payload, length);
}

void MqttConnector::setup() {
    mqttClient.setServer(strdup(server->getValue().c_str()), port->getValue());
    mqttClient.setCallback(callback);
    logSettings();
}

void MqttConnector::loop(bool firstLoop) {
    if (wifi->getNetworkStatus() == NetworkStatus::STATION && enable->getValue()) {
        if (!mqttClient.connected()) {
            logger->info(F("Connect to MQTT Server "));
            if (mqttClient.connect(clientId->getValue().c_str(), username->getValue().c_str(), password->getValue().c_str())) {
                logger->info(F("Mqtt Connection established."));
                eventManager.fireEvent(new MqttConnectedEvent());
                return;
            } else {
                logger->warn(F("Mqtt Connection failed with state %d"), mqttClient.state());
                delay(1000);
            }
        }
        mqttClient.loop();
    }
}

void MqttConnector::registerSubscribe(Element *element_, String topic_) {
    String topic = prefix->getValue() + topic_;
    subscribeElements[topic] = new MqttConnectorSubscribeElement(element_);
}

void MqttConnector::registerPublish(Element *element_, String topic_) {
    String topic = prefix->getValue() + topic_;
    publishElements[element_->id] = new MqttConnectorPublishElement(element_, topic);
}

void MqttConnector::registerGlobalSubscribe(Element *element_, String topic_) {
    subscribeElements[topic_] = new MqttConnectorSubscribeElement(element_);
}

void MqttConnector::registerGlobalPublish(Element *element_, String topic_) {
    publishElements[element_->id] = new MqttConnectorPublishElement(element_, topic_);
}

void MqttConnector::onMqttMessage(char *topic_, byte *payload, unsigned int length) {
    char cPayLoad[length + 1];
    for (int i = 0; i < length; i++) {
        cPayLoad[i] = (char)payload[i];
    }
    cPayLoad[length] = 0;
    logger->debug(F("Mqtt : Message Topic : %s Value : %s"), topic_, cPayLoad);

    String topic = String(topic_);
    if (subscribeElements.find(topic) != subscribeElements.end()) {
        MqttConnectorSubscribeElement *ele = subscribeElements[topic];
        if (ele->element->getType() == BOOL_TYPE) {
            bool newValue = (!strcasecmp(cPayLoad, "true") || !strcasecmp(cPayLoad, "on") || !strcasecmp(cPayLoad, "1"));
            eventManager.fireEvent(new NewBoolValueEvent(ele->element->id, newValue));
        } else if (ele->element->getType() == STRING_TYPE) {
            eventManager.fireEvent(new NewStringValueEvent(ele->element->id, String(cPayLoad)));
        } else if (ele->element->getType() == LONG_TYPE) {
            eventManager.fireEvent(new NewLongValueEvent(ele->element->id, atol (cPayLoad)));
        }
    }
}

void MqttConnector::onMqttConnected() {
    //--- Publish all values to mqtt ----------------------------------
    for (const auto &pair : publishElements) {
        switch (pair.second->element->getType()) {
        case BOOL_TYPE:
            mqttClient.publish(pair.second->topic.c_str(), (pair.second->boolValue ? "true" : "false"), true);
            break;
        case LONG_TYPE:
            mqttClient.publish(pair.second->topic.c_str(), String(pair.second->longValue).c_str(), true);
            break;
        case STRING_TYPE:
            mqttClient.publish(pair.second->topic.c_str(), pair.second->stringValue.c_str(), true);
            break;
        }
    }

    //--- Subscribe all elements ----------------------------------
    for (const auto &pair : subscribeElements) {
        mqttClient.subscribe(pair.first.c_str(), 1);
    }
}

void MqttConnector::logSettings() {
    logger->info(F("---- Mqtt settings : ------"));
    logger->info(F("Enabled               : %s"), (enable->getValue() ? "true" : "false"));
    logger->info(F("Server                : %s"), server->getValue().c_str());
    logger->info(F("Port                  : %d"), port->getValue());
    logger->info(F("Prefix                : %s"), prefix->getValue().c_str());
    logger->info(F("ClientId              : %s"), clientId->getValue().c_str());
    logger->info(F("Username              : %s"), username->getValue().c_str());
    logger->info(F("Password              : %s"), password->getValue().c_str());
}

MqttConnector *_mqttConnector = nullptr;

MqttConnector *getMqttConnector() {
    if (!_mqttConnector) {
        _mqttConnector = new MqttConnector();
    }
    return _mqttConnector;
}
