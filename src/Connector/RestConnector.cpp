#include "RestConnector.h"
#include <Connector/DigitalInConnector.h>
#include <Core/EventManager.h>
#include <Core/WebServer.h>
#include <OpenIot.h>

#include <Modules/Wifi.h>
#include <Preferences.h>
#include <WiFi.h>

/**
 * The RestConnector provides an access through a REST Interface for registered elements
 *
 *
 * The URL for querying an Element is
 *     http://<ip of device>/api/<ElementId>/
 *
 * The URL for setting an Element ist
 *
 *     http://<ip of device>/api/<ElementId>/<payload>
 *
 */
RestConnector::RestConnector() : Connector() {
    wifi->depend();
    openIot.registerConnector(this);

    /**
     * Register Eventlisteners for the different types. If an event is received and the element is registered to the service, 
     * the value is stored for the next quzery
     */
    eventManager.addListener<NewBoolValueEvent>(NewBoolValue, [this](NewBoolValueEvent *event) {
        if (restElements.find(event->destination) != restElements.end()) {
            RestConnectorElement *ele = restElements[event->destination];
            if (ele->element->getType() == BOOL_TYPE) {
                ele->boolValue = event->newValue;
            } else {
                logger->warn(F("Invalid type for REST Element %s : %d expected BOOL (%d)"), event->destination.c_str(), ele->element->getType(), BOOL_TYPE);
            }
        }
    });

    eventManager.addListener<NewLongValueEvent>(NewLongValue, [this](NewLongValueEvent *event) {
        if (restElements.find(event->destination) != restElements.end()) {
            RestConnectorElement *ele = restElements[event->destination];
            if (ele->element->getType() == LONG_TYPE) {
                ele->longValue = event->newValue;
            } else {
                logger->warn(F("Invalid type for REST Element %s : %d expected LONG (%d)"), event->destination.c_str(), ele->element->getType(), LONG_TYPE);
            }
        }
    });

    eventManager.addListener<NewStringValueEvent>(NewStringValue, [this](NewStringValueEvent *event) {
        if (restElements.find(event->destination) != restElements.end()) {
            RestConnectorElement *ele = restElements[event->destination];
            if (ele->element->getType() == STRING_TYPE) {
                ele->stringValue = event->newValue;
            } else {
                logger->warn(F("Invalid type for REST Element %s : %d expected STRING (%d)"), event->destination.c_str(), ele->element->getType(), STRING_TYPE);
            }
        }
    });
}

/**
 * Setup registers the REST handler to the webserver 
 */
void RestConnector::setup() {
    webServer->on("/api", HTTP_GET, [this](AsyncWebServerRequest *request_) {

        if (request_->url().length() > 500) {
            request_->send(401, "text/html", "Request too long !");
            return;
        }

        char urlBuffer[500 + 1];
        strcpy(urlBuffer, request_->url().c_str());

        //--- SKIP /api/ -----------------------------
        char *elementId = urlBuffer + 5;

        //--- find the delimiter between id and payload ------
        char *payload = nullptr;
        char *endId = strchr(elementId, '/');
        if (endId != NULL) {
            *endId = 0;
            payload = endId + 1;
        }

        if (restElements.find(elementId) != restElements.end()) {
            RestConnectorElement *ele = restElements[elementId];
            if (payload == nullptr) {
                //--- No payload send value ---------------------
                switch (ele->element->getType()) {
                case BOOL_TYPE:
                    if (ele->boolValue) {
                        request_->send(200, "text/html", "ON");
                    } else {
                        request_->send(200, "text/html", "OFF");
                    }
                    break;
                case STRING_TYPE:
                    request_->send(200, "text/html", ele->stringValue);
                    break;
                case LONG_TYPE:
                    request_->send(200, "text/html", String(ele->longValue));
                    break;
                }
            } else {
                //--- payload : set value -----------------------
                switch (ele->element->getType()) {
                case BOOL_TYPE:
                    eventManager.fireEvent(new NewBoolValueEvent(ele->element->id, (!strcasecmp(payload, "true") || !strcasecmp(payload, "on") || !strcasecmp(payload, "1"))));
                    break;
                case STRING_TYPE:
                    eventManager.fireEvent(new NewStringValueEvent(ele->element->id, payload));
                    break;
                case LONG_TYPE:
                    eventManager.fireEvent(new NewLongValueEvent(ele->element->id, atol(payload)));
                    break;
                }
                request_->send(200, "text/html", "OK");
            }
        } else {
            request_->send(404, "text/html", "Not found");
        }
        return;
    });
    webServer->begin();
    logSettings();
}

void RestConnector::registerElement(Element *element_, String topic_) {
    restElements[topic_] = new RestConnectorElement(element_, topic_);
}

void RestConnector::logSettings() {
    logger->info(F("---- Rest settings : ------"));
    logger->info(F("Enabled               : %s"), (enable->getValue() ? "true" : "false"));
}

RestConnector *_restConnector = nullptr;

RestConnector *getRestConnector() {
    if (!_restConnector) {
        _restConnector = new RestConnector();
    }
    return _restConnector;
}
