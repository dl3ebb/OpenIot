#ifndef WEB_ELEMENT_H
#define WEB_ELEMENT_H
#include "WebForm.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Core/Element.h>

class WebElement {
  public:
    Element *element;
    String error;
    WebForm *form;

    WebElement(Element *element_, WebForm *form_) : element(element_), form(form_) {
        if (form_ != nullptr) {
            form_->registerFormElement(this);
        }
    };
    void setError(String error_) { error = error_; }
    virtual void setup() = 0;
    virtual void toJsonObject(JsonObject obj) = 0;
    virtual void handleWebEvent(JsonDocument json) = 0;
};

#endif