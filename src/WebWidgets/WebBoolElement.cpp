#include "WebBoolElement.h"
#include <Connector/WebConnector.h>

void WebBoolElement::setup() {
    eventManager.addListener<NewBoolValueEvent>(NewBoolValue, this->element->id, [this](NewBoolValueEvent *evt) {
        this->value = evt->newValue;
        if (form != nullptr) {
            form->__checkFormData();
        } else {
            webConnector->sendElement(this);
        }
    });
}

void WebBoolElement::toJsonObject(JsonObject obj) {
    obj["value"] = value;
    obj["error"] = error;
}

void WebBoolElement::handleWebEvent(JsonDocument json) {
    if (json["Message"] == "ValueChange") {
        bool newValue = (json["value"] == "true" || json["value"] == "on" || json["value"] == "1");
        if (newValue != this->value) {
            this->value = newValue;
            eventManager.fireEvent(new NewBoolValueEvent(this->element->id, this->value));
        }
    }
};
