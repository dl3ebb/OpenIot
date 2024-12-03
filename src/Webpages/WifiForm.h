#ifndef WIFI_FORM_H
#define WIFI_FORM_H
#include <WebWidgets/WebBoolElement.h>
#include <WebWidgets/WebButtonElement.h>
#include <WebWidgets/WebSelectElement.h>
#include <WebWidgets/WebStringElement.h>

class WifiForm : public WebForm {

  private:
    WebBoolElement *enablePrimary;
    WebSelectElement *ssid1;
    WebStringElement *password1;
    WebBoolElement *enableSecondary;
    WebSelectElement *ssid2;
    WebStringElement *password2;
    WebButtonElement *save;

  public:
    WifiForm();
    void setup() override;
    bool checkFormData() override;
    void submit() override;
};

#define wifiForm getWifiForm()

WifiForm *getWifiForm();

#endif
