#include "WebButtonElement.h"
#include <Connector/WebConnector.h>

void WebButtonElement::setup() {
}

void WebButtonElement::toJsonObject(JsonObject obj) {
    obj["enabled"] = enabled;
}

void WebButtonElement::enable() {
    if (!this->enabled) {
        this->enabled = true;
        webConnector->sendElement(this);
    }
}

void WebButtonElement::disable() {
    if (this->enabled) {
        this->enabled = false;
        webConnector->sendElement(this);
    }
}

void WebButtonElement::handleWebEvent(JsonDocument json) {
    if (json["Message"] == "Click") {
        eventManager.fireEvent(new WebButtonClickedEvent(this->element->id));
    }
};
