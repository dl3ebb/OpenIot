#ifndef WEB_BOOL_ELEMENT_H
#define WEB_BOOL_ELEMENT_H
#include "WebElement.h"

class WebBoolElement : public WebElement {
  public:
    bool value;

    WebBoolElement(BoolElement *element, WebForm *form_) : WebElement(element, form_), value(false) {}
    void setValue(bool newValue) { static_cast<BoolElement *>(element)->setValue(newValue); }
    bool getValue() { return value; }
    void setup() override;
    void toJsonObject(JsonObject obj) override;
    void handleWebEvent(JsonDocument json) override;
};

#endif