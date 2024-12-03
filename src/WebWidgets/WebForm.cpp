
#include "WebForm.h"
#include "WebButtonElement.h"
#include <Core/EventManager.h>
#include <ESPAsyncWebServer.h>

extern AsyncWebSocket ws;

void WebForm::setSubmitButton(WebButtonElement *submitButton) {
    this->submitButton = submitButton;
    eventManager.addListener<WebButtonClickedEvent>(WebButtonClicked, submitButton->element->id, [this](WebButtonClickedEvent *event_) {
        if (__checkFormData()) {
            submit();
        }
    });
}

bool WebForm::__checkFormData() {
    bool result = checkFormData();
    JsonDocument doc;
    doc["Message"] = "SendData";
    for (auto element : formElements) {
        JsonObject obj = doc[element->element->id].to<JsonObject>();
        element->toJsonObject(obj);
    }
    String json;
    serializeJson(doc, json);
    ws.textAll(json);
    return result;
}

void WebForm::__setup() {
    setup();
    checkFormData();
}
