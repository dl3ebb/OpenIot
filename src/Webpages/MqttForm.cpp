#include "MqttForm.h"
#include <Connector/MqttConnector.h>
#include <Connector/WebConnector.h>

MqttForm::MqttForm() : WebForm() {
    enableMqtt = webConnector->registerElement(new BoolElement("_SYS_MQTT_ENABLE"), this);
    server = webConnector->registerElement(new StringElement("_SYS_MQTT_SERVER"), this);
    port = webConnector->registerElement(new LongElement("_SYS_MQTT_SERVER_PORT"), this);
    clientId = webConnector->registerElement(new StringElement("_SYS_MQTT_CLIENT_ID"), this);
    userName = webConnector->registerElement(new StringElement("_SYS_MQTT_USER_NAME"), this);
    password = webConnector->registerElement(new StringElement("_SYS_MQTT_PASSWORD"), this);
    prefix = webConnector->registerElement(new StringElement("_SYS_MQTT_PREFIX"), this);
    save = webConnector->registerElement(new ButtonElement("_SYS_MQTT_SAVE"), this);
    this->setSubmitButton(save);
}

bool MqttForm::checkFormData() {
    bool hasError = false;

    if (enableMqtt->getValue()) {
        if (server->getValue() == "") {
            server->setError(F("Server may not be empty"));
            hasError = true;
        } else {
            server->setError("");
        }

        if (port->getValue() <= 0) {
            port->setError(F("Invalid port number"));
            hasError = true;
        } else {
            port->setError("");
        }

        if (clientId->getValue() == "") {
            clientId->setError(F("Client-Id may not be empty"));
            hasError = true;
        } else {
            clientId->setError("");
        }
    } else {
        server->setError("");
        port->setError("");
        clientId->setError("");
    }

    if (hasError) {
        save->disable();
        return false;
    } else {
        save->enable();
        return true;
    }
}

void MqttForm::setup() {
    enableMqtt->setValue(mqttConnector->getEnable());
    server->setValue(mqttConnector->getServer());
    port->setValue(mqttConnector->getPort());
    clientId->setValue(mqttConnector->getClientId());
    userName->setValue(mqttConnector->getUsername());
    password->setValue(mqttConnector->getPassword());
    prefix->setValue(mqttConnector->getPrefix());
}

void MqttForm::submit() {
    mqttConnector->setEnable(enableMqtt->getValue());
    if (mqttConnector->getEnable()) {
        mqttConnector->setServer(server->getValue().c_str());
        mqttConnector->setPort(port->getValue());
        mqttConnector->setClientId(clientId->getValue().c_str());
        mqttConnector->setUsername(userName->getValue().c_str());
        mqttConnector->setPassword(password->getValue().c_str());
        mqttConnector->setPrefix(prefix->getValue().c_str());
    } else {
        mqttConnector->setServer("");
        mqttConnector->setPort(1883);
        mqttConnector->setClientId("");
        mqttConnector->setUsername("");
        mqttConnector->setPassword("");
        mqttConnector->setPrefix("");
    }
}

MqttForm *_mqttForm = nullptr;

MqttForm *getMqttForm() {
    if (!_mqttForm) {
        _mqttForm = new MqttForm();
    }
    return _mqttForm;
}
