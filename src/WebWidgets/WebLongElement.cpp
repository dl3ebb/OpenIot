#include "WebLongElement.h"
#include <Connector/WebConnector.h>

void WebLongElement::setup() {
    eventManager.addListener<NewLongValueEvent>(NewLongValue, this->element->id, [this](NewLongValueEvent *evt) {
        this->value = evt->newValue;
        if (form != nullptr) {
            form->__checkFormData();
        } else {
            webConnector->sendElement(this);
        }
    });
}

void WebLongElement::toJsonObject(JsonObject obj) {
    obj["value"] = value;
    obj["error"] = error;
}

void WebLongElement::handleWebEvent(JsonDocument json) {
    if (json["Message"] == "ValueChange") {
        long newValue = json["value"].as<long>();
        if (newValue != this->value) {
            this->value = newValue;
            eventManager.fireEvent(new NewLongValueEvent(this->element->id, this->value));
        }
    }
};
