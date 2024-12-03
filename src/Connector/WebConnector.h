#ifndef WEB_CONNECTOR_H
#define WEB_CONNECTOR_H

#include "Connector.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Core/Element.h>
#include <Core/EventManager.h>
#include <WebWidgets/WebBoolElement.h>
#include <WebWidgets/WebButtonElement.h>
#include <WebWidgets/WebCountdownElement.h>
#include <WebWidgets/WebLongElement.h>
#include <WebWidgets/WebProgressDialogElement.h>
#include <WebWidgets/WebSelectElement.h>
#include <WebWidgets/WebStringElement.h>
#include <map>
#include <vector>

/**
 * The web connector provides the connection betweenm the element and a webclient via websocket.
 */
class WebConnector : public Connector {
    std::map<String, WebElement *> elements;

  public:
    WebConnector();
    void setup() override;
    void loop(bool firstLoop) override {}

    //--- Register the different element types ----------------------------------------
    WebBoolElement *registerElement(BoolElement *element, WebForm *form = nullptr);
    WebStringElement *registerElement(StringElement *element, WebForm *form = nullptr);
    WebLongElement *registerElement(LongElement *element, WebForm *form = nullptr);
    WebSelectElement *registerElement(SelectElement *element, WebForm *form = nullptr);
    WebButtonElement *registerElement(ButtonElement *element_, WebForm *form = nullptr);
    WebProgressDialogElement *registerElement(ProgressDialogElement *element_, WebForm *form = nullptr);
    WebCountdownElement *registerElement(CountdownElement *element_, WebForm *form = nullptr);

    //--- Sends an element to the web clients ------------------------------------------
    void sendElement(WebElement *element);

    //--- handles the incoming web events from the clients -----------------------------
    void handleWebEvent(JsonDocument json);
};

/**
 * WebClientConnectedEvent is fired when a new Webclient connects to the device
 */
class WebClientConnectedEvent : public Event {
  public:
    String toStringResult;
    unsigned int clientId;
    char ipAddress[16];

    WebClientConnectedEvent(unsigned int clientId_, IPAddress ip) : Event(WebClientConnected, ""), clientId(clientId_) {
        sprintf(ipAddress, "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
    }

    void cleanup() override {
        delete (this);
    }

    String toString() override {
        if (toStringResult.isEmpty()) {
            toStringResult = "WebClientConnectedEvent";
            toStringResult += " ClientId : ";
            toStringResult += clientId;
            toStringResult += " Ip-Address : ";
            toStringResult += ipAddress;
        }
        return toStringResult;
    }
};

#define webConnector getWebConnector()

WebConnector *getWebConnector();

#endif