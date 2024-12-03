#ifndef WEB_STRING_ELEMENT_H
#define WEB_STRING_ELEMENT_H
#include "WebElement.h"
#include <Arduino.h>

class WebStringElement : public WebElement {
  public:
    String value;

    WebStringElement(StringElement *element, WebForm *form_) : WebElement(element, form_) {}
    void setValue(String newValue) { static_cast<StringElement *>(element)->setValue(newValue); }
    String getValue() { return value; }
    void setup() override;
    void toJsonObject(JsonObject obj) override;
    void handleWebEvent(JsonDocument json) override;
};

#endif