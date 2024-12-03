#include "LoggingForm.h"
#include <Connector/WebConnector.h>
#include <Logger/SyslogLogger.h>

LoggingForm::LoggingForm() : WebForm() {
    syslogEnable = webConnector->registerElement(new BoolElement("_SYS_SYSLOG_ENABLE"), this);
    syslogServer = webConnector->registerElement(new StringElement("_SYS_SYSLOG_SERVER"), this);
    syslogPort = webConnector->registerElement(new LongElement("_SYS_SYSLOG_PORT"), this);
    save = webConnector->registerElement(new ButtonElement("_SYS_SYSLOG_SAVE"), this);
    setSubmitButton(save);
}

bool LoggingForm::checkFormData() {
    bool hasError = false;

    if (syslogEnable->getValue()) {
        if (syslogServer->getValue() == "") {
            syslogServer->setError(F("Servername may not be empty."));
            hasError = true;
        } else {
            syslogServer->setError("");
        }
        if (syslogPort->getValue() <= 0) {
            syslogPort->setError(F("Invalid port number"));
            hasError = true;
        } else {
            syslogPort->setError("");
        }
    } else {
        syslogServer->setError("");
        syslogServer->setError("");
    }

    if (hasError) {
        save->disable();
        return false;
    } else {
        save->enable();
        return true;
    }
}

void LoggingForm::setup() {
    syslogEnable->setValue(syslogLogger->getEnable());
    syslogServer->setValue(syslogLogger->getLogServer());
    syslogPort->setValue(syslogLogger->getServerPort());
    checkFormData();
}

void LoggingForm::submit() {
    Serial.println("Submit");
    syslogLogger->setEnable(syslogEnable->getValue());
    if (syslogLogger->getEnable()) {
        syslogLogger->setLogServer(syslogServer->getValue().c_str());
        syslogLogger->setServerPort(syslogPort->getValue());
    } else {
        syslogLogger->setLogServer("");
        syslogLogger->setServerPort(514);
    }
}

LoggingForm *_loggingForm = nullptr;

LoggingForm *getLoggingForm() {
    if (!_loggingForm) {
        _loggingForm = new LoggingForm();
    }
    return _loggingForm;
}
