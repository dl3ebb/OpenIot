#ifndef HOTSPOT_FORM_H
#define HOTSPOT_FORM_H
#include <WebWidgets/WebBoolElement.h>
#include <WebWidgets/WebButtonElement.h>
#include <WebWidgets/WebStringElement.h>

class HotspotForm : public WebForm {
    WebStringElement *ssid;
    WebStringElement *password;
    WebBoolElement *openAccessPoint;
    WebButtonElement *submitButton;

  public:
    HotspotForm();
    void setup() override;
    bool checkFormData() override;
    void submit() override;
};

#define hotspotForm getHotspotForm()

HotspotForm *getHotspotForm();

#endif