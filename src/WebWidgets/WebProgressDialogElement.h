#ifndef WEB_PROGRESS_DIALOG_ELEMENT_H
#define WEB_PROGRESS_DIALOG_ELEMENT_H
#include "WebElement.h"

class WebProgressDialogElement : public WebElement {
  private:
    bool visible;
    String label;
    int maxValue;
    int currentValue;
    int percentage;

  public:
    WebProgressDialogElement(ProgressDialogElement *element, WebForm *form_) : WebElement(element, form_),
                                                                               visible(false),
                                                                               label(""),
                                                                               maxValue(0),
                                                                               currentValue(0) {}

    void setup() override;
    void toJsonObject(JsonObject obj) override;
    void handleWebEvent(JsonDocument json) override {};

    void show();
    void hide();
    void setLabel(String label_);
    void setMaxValue(int max_);
    void setCurrentValue(int current_);
};

#endif