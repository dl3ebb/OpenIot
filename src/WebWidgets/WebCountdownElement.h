#ifndef WEB_COUNTDOWN_ELEMENT_H
#define WEB_COUNTDOWN_ELEMENT_H
#include "WebElement.h"

class WebCountdownElement : public WebElement {
  private:
    bool started;

  public:
    WebCountdownElement(CountdownElement *element, WebForm *form_) : WebElement(element, form_),
                                                                     started(false) {}

    void setup() override;
    void toJsonObject(JsonObject obj) override;
    void handleWebEvent(JsonDocument json) override {};
    void start();
};

#endif