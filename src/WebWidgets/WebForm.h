#ifndef WEB_FORM_H
#define WEB_FORM_H
#include <vector>

class WebElement;
class WebButtonElement;

class WebForm {
  private:
    std::vector<WebElement *> formElements;
    WebButtonElement *submitButton;

  public:
    void registerFormElement(WebElement *_element) {
        formElements.push_back(_element);
    }
    void setSubmitButton(WebButtonElement *submitButton);
    bool __checkFormData();
    void __setup();
    virtual void setup() = 0;
    virtual bool checkFormData() = 0;
    virtual void submit() = 0;
};

#endif
