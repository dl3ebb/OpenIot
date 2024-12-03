#ifndef DEVICE_FORM_H
#define DEVICE_FORM_H
#include <WebWidgets/WebButtonElement.h>
#include <WebWidgets/WebStringElement.h>
/**
 * Device Form is a Web Form to set the devicename
 */
class DeviceForm : public WebForm {

  private:
    WebStringElement *deviceName;
    WebButtonElement *save;

  public:
    DeviceForm();
    bool checkFormData() override;
    void submit() override;
    void setup() override;
};

#define deviceForm getDeviceForm()

DeviceForm *getDeviceForm();

#endif