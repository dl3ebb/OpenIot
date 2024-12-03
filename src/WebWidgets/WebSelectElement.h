#ifndef WEB_SELECT_ELEMENT_H
#define WEB_SELECT_ELEMENT_H
#include "WebElement.h"
#include <Arduino.h>

struct SelectboxOption {
    String value;
    String text;
};

class WebSelectElement : public WebElement {
  public:
    String value;
    std::vector<SelectboxOption> options;

    WebSelectElement(SelectElement *element, WebForm *form_) : WebElement(element, form_) {}
    void setValue(String newValue) { static_cast<SelectElement *>(element)->setValue(newValue); }
    void setOptions(std::vector<SelectboxOption> options_) { options = options_; }

    String getValue() { return value; }
    void setup() override;
    void toJsonObject(JsonObject obj) override;
    void handleWebEvent(JsonDocument json) override;
};

#endif