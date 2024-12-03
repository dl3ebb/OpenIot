#ifndef WEB_BUTTON_ELEMENT_H
#define WEB_BUTTON_ELEMENT_H
#include "WebElement.h"

class WebButtonElement : public WebElement {
  private:
    bool enabled;

  public:
    WebButtonElement(ButtonElement *element, WebForm *form_) : WebElement(element, form_), enabled(true) {}

    void setup() override;
    void toJsonObject(JsonObject obj) override;
    void handleWebEvent(JsonDocument json) override;
    void enable();
    void disable();
};

class WebButtonClickedEvent : public Event {
  public:
    String toStringResult;

    WebButtonClickedEvent(String destination_) : Event(WebButtonClicked, destination_) {}
    void cleanup() override {
        delete (this);
    }

    String toString() override {
        if (toStringResult.isEmpty()) {
            toStringResult = "WebButtonClickedEvent (";
            toStringResult += type;
            toStringResult += ")";
        }
        return toStringResult.c_str();
    }
};

#endif