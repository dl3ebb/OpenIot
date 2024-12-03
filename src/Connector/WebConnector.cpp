#include "WebConnector.h"
#include <Core/EventManager.h>
#include <Core/WebServer.h>
#include <Modules/Wifi.h>
#include <Modules/Wifi.h>
#include <OpenIot.h>
#include <SPIFFS.h>

AsyncWebSocket ws("/ws");

/**
 * Callback for incoming messages. The data is parsed from json to a JsonDocument and the passed to the webConnector for further processing
 */
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        data[len] = 0;
        logger->debug(F("Webevent : '%s'"), data);
        JsonDocument doc;
        deserializeJson(doc, data);
        webConnector->handleWebEvent(doc);
    }
}

/*
 * callback for websocket events.
 */
void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    switch (type) {
    case WS_EVT_CONNECT:
        eventManager.fireEvent(new WebClientConnectedEvent(client->id(), client->remoteIP()));
        break;
    case WS_EVT_DISCONNECT:
        logger->info(F("WebSocket client %d disconnected."), client->id());
        break;
    case WS_EVT_DATA:
        handleWebSocketMessage(arg, data, len);
        break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        break;
    }
}

WebConnector::WebConnector() : Connector() {
    /**
     * Register an listener for the webclientconnected event. This will send all registered elements to the client.
     */
    eventManager.addListener<WebClientConnectedEvent>(WebClientConnected, [this](WebClientConnectedEvent *event) {
        JsonDocument doc;
        doc["Message"] = "ClientConnected";
        for (const auto &pair : elements) {
            JsonObject obj = doc[pair.second->element->id].to<JsonObject>();
            pair.second->toJsonObject(obj);
        }
        String json;
        serializeJson(doc, json);
        ws.text(event->clientId, json);
    });
    wifi->depend();
    openIot.registerConnector(this);
}


void WebConnector::setup() {
    /**
     * Initialize the SPIFFS that contains the static files for the web application 
     * and tell the webserver to serve them
     */
    if (!SPIFFS.begin(true)) {
        logger->warn(F("An Error has occurred while mounting SPIFFS. Webserver not initialized!"));
        return;
    }
    webServer->serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
    webServer->begin();

    //--- Register the callback -------------------------------------------------
    ws.onEvent(onWebSocketEvent);
    webServer->addHandler(&ws);

    //--- Initialize the web Elements -------------------------------------------
    for (const auto &pair : elements) {
        pair.second->setup();
    }
}

/**
 * Registration for the different element types. These connect the internal type (bool/string/long/void) 
 * to an element that may have additional properties for the web interface 
 */
WebBoolElement *WebConnector::registerElement(BoolElement *element_, WebForm *form) {
    WebBoolElement *webElement = new WebBoolElement(element_, form);
    elements[webElement->element->id] = webElement;
    return webElement;
}

WebStringElement *WebConnector::registerElement(StringElement *element_, WebForm *form) {
    WebStringElement *webElement = new WebStringElement(element_, form);
    elements[webElement->element->id] = webElement;
    return webElement;
}

WebLongElement *WebConnector::registerElement(LongElement *element_, WebForm *form) {
    WebLongElement *webElement = new WebLongElement(element_, form);
    elements[webElement->element->id] = webElement;
    return webElement;
}

WebButtonElement *WebConnector::registerElement(ButtonElement *element_, WebForm *form) {
    WebButtonElement *webElement = new WebButtonElement(element_, form);
    elements[webElement->element->id] = webElement;
    return webElement;
}

WebSelectElement *WebConnector::registerElement(SelectElement *element_, WebForm *form) {
    WebSelectElement *webElement = new WebSelectElement(element_, form);
    elements[webElement->element->id] = webElement;
    return webElement;
}

WebProgressDialogElement *WebConnector::registerElement(ProgressDialogElement *element_, WebForm *form) {
    WebProgressDialogElement *webElement = new WebProgressDialogElement(element_, form);
    elements[webElement->element->id] = webElement;
    return webElement;
}

WebCountdownElement *WebConnector::registerElement(CountdownElement *element_, WebForm *form) {
    WebCountdownElement *webElement = new WebCountdownElement(element_, form);
    elements[webElement->element->id] = webElement;
    return webElement;
}

/**
 * This sends a single element to the browser 
 */
void WebConnector::sendElement(WebElement *element) {
    JsonDocument doc;
    doc["Message"] = "SendData";
    JsonObject obj = doc[element->element->id].to<JsonObject>();
    element->toJsonObject(obj);
    String json;
    serializeJson(doc, json);
    ws.textAll(json);
}

/**
 * Habndle incoming web requests. 
 */
void WebConnector::handleWebEvent(JsonDocument json) {
    String id = json["id"];
    if (elements.find(id.c_str()) != elements.end()) {
        elements[id.c_str()]->handleWebEvent(json);
    }
}


WebConnector *_webConnector = nullptr;

WebConnector *getWebConnector() {
    if (!_webConnector) {
        _webConnector = new WebConnector();
    }
    return _webConnector;
}


