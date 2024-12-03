#ifndef WEB_LONG_ELEMENT_H
#define WEB_LONG_ELEMENT_H
#include "WebElement.h"
#include <Arduino.h>

class WebLongElement : public WebElement {
  public:
    long value;

    WebLongElement(LongElement *element, WebForm *form_) : WebElement(element, form_) {}
    void setValue(long newValue) { static_cast<LongElement *>(element)->setValue(newValue); }
    long getValue() { return value; }
    void setup() override;
    void toJsonObject(JsonObject obj) override;
    void handleWebEvent(JsonDocument json) override;
};

#endif