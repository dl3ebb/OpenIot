#ifndef LOGGING_FORM_H
#define LOGGING_FORM_H
#include <WebWidgets/WebBoolElement.h>
#include <WebWidgets/WebButtonElement.h>
#include <WebWidgets/WebLongElement.h>
#include <WebWidgets/WebStringElement.h>

class LoggingForm : public WebForm {

  private:
    WebBoolElement *syslogEnable;
    WebStringElement *syslogServer;
    WebLongElement *syslogPort;
    WebButtonElement *save;

  public:
    LoggingForm();
    bool checkFormData() override;
    void submit() override;
    void setup() override;
};

#define loggingForm getLoggingForm()

LoggingForm *getLoggingForm();

#endif