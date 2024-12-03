#include "WebSelectElement.h"
#include <Connector/WebConnector.h>

void WebSelectElement::setup() {
    eventManager.addListener<NewStringValueEvent>(NewStringValue, this->element->id, [this](NewStringValueEvent *evt) {
        this->value = evt->newValue;
        if (form != nullptr) {
            form->__checkFormData();
        } else {
            webConnector->sendElement(this);
        }
    });
}

void WebSelectElement::toJsonObject(JsonObject obj) {
    obj["value"] = value;
    obj["error"] = error;
    int i = 0;
    for (SelectboxOption opt : options) {
        obj["options"][i]["value"] = opt.value;
        obj["options"][i]["text"] = opt.text;
        ++i;
    }
}

void WebSelectElement::handleWebEvent(JsonDocument json) {
    if (json["Message"] == "ValueChange") {
        String newValue = json["value"].as<String>();
        if (newValue != this->value) {
            this->value = newValue;
            eventManager.fireEvent(new NewStringValueEvent(this->element->id, this->value));
        }
    }
};
