#ifndef NTP_FORM_H
#define NTP_FORM_H
#include <WebWidgets/WebBoolElement.h>
#include <WebWidgets/WebButtonElement.h>
#include <WebWidgets/WebLongElement.h>
#include <WebWidgets/WebStringElement.h>

class NtpForm : public WebForm {

  private:
    WebBoolElement *ntpEnable;
    WebStringElement *ntpServer1;
    WebStringElement *ntpServer2;
    WebStringElement *ntpServer3;
    WebLongElement *ntpOffsetGmt;
    WebLongElement *ntpOffsetDst;
    WebButtonElement *save;

  public:
    NtpForm();
    void setup() override;
    bool checkFormData() override;
    void submit() override;
};

#define ntpForm getNtpForm()

NtpForm *getNtpForm();

#endif
