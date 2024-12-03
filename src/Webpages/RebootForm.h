#ifndef REBOOT_FORM_H
#define REBOOT_FORM_H
#include <WebWidgets/WebButtonElement.h>

class RebootForm : public WebForm {

  private:
    WebButtonElement *reboot;
    WebButtonElement *reset;

  public:
    RebootForm();
    bool checkFormData() override { return false; };
    void submit() override {};
    void setup() override {};
};

#define rebootForm getRebootForm()

RebootForm *getRebootForm();

#endif
