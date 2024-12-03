#include "WebStringElement.h"
#include <Connector/WebConnector.h>

void WebStringElement::setup() {
    eventManager.addListener<NewStringValueEvent>(NewStringValue, this->element->id, [this](NewStringValueEvent *evt) {
        this->value = evt->newValue;
        if (form != nullptr) {
            form->__checkFormData();
        } else {
            webConnector->sendElement(this);
        }
    });
}

void WebStringElement::toJsonObject(JsonObject obj) {
    obj["value"] = value;
    obj["error"] = error;
}

void WebStringElement::handleWebEvent(JsonDocument json) {
    if (json["Message"] == "ValueChange") {
        String newValue = json["value"].as<String>();
        if (newValue != this->value) {
            this->value = newValue;
            eventManager.fireEvent(new NewStringValueEvent(this->element->id, this->value));
        }
    }
    if (json["Message"] == "Click") {
        eventManager.fireEvent(new WebButtonClickedEvent(this->element->id));
    }
};
