#ifndef MQTT_FORM_H
#define MQTT_FORM_H
#include <WebWidgets/WebBoolElement.h>
#include <WebWidgets/WebButtonElement.h>
#include <WebWidgets/WebLongElement.h>
#include <WebWidgets/WebSelectElement.h>
#include <WebWidgets/WebStringElement.h>

class MqttForm : public WebForm {

  private:
    WebBoolElement *enableMqtt;
    WebStringElement *server;
    WebLongElement *port;
    WebStringElement *clientId;
    WebStringElement *userName;
    WebStringElement *password;
    WebStringElement *prefix;
    WebButtonElement *save;

  public:
    MqttForm();
    bool checkFormData() override;
    void submit() override;
    void setup() override;
};

#define mqttForm getMqttForm()

MqttForm *getMqttForm();

#endif
