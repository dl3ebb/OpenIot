#include "NtpForm.h"
#include <Connector/WebConnector.h>
#include <Modules/Ntp.h>

NtpForm::NtpForm() : WebForm() {
    ntpEnable = webConnector->registerElement(new BoolElement("_SYS_NTP_ENABLE"), this);
    ntpServer1 = webConnector->registerElement(new StringElement("_SYS_NTP_SERVER1"), this);
    ntpServer2 = webConnector->registerElement(new StringElement("_SYS_NTP_SERVER2"), this);
    ntpServer3 = webConnector->registerElement(new StringElement("_SYS_NTP_SERVER3"), this);
    ntpOffsetGmt = webConnector->registerElement(new LongElement("_SYS_NTP_OFFSET_GMT"), this);
    ntpOffsetDst = webConnector->registerElement(new LongElement("_SYS_NTP_OFFSET_DST"), this);
    save = webConnector->registerElement(new ButtonElement("_SYS_NTP_SAVE"), this);
    setSubmitButton(save);
}

bool NtpForm::checkFormData() {
    bool hasError = false;

    if (ntpEnable->getValue()) {
        if (ntpServer1->getValue() == "") {
            ntpServer1->setError(F("Server may not be empty"));
            hasError = true;
        } else {
            ntpServer1->setError("");
        }
    } else {
        ntpServer1->setError("");
    }

    if (hasError) {
        save->disable();
        return false;
    } else {
        save->enable();
        return true;
    }
}

void NtpForm::setup() {
    ntpEnable->setValue(ntp->getEnable());
    ntpServer1->setValue(ntp->getServer1());
    ntpServer2->setValue(ntp->getServer2());
    ntpServer3->setValue(ntp->getServer3());
    ntpOffsetGmt->setValue(ntp->getOffsetGmt());
    ntpOffsetDst->setValue(ntp->getOffsetDst());
}

void NtpForm::submit() {
    ntp->setEnable(ntpEnable->getValue());
    if (ntp->getEnable()) {
        ntp->setServer1(ntpServer1->getValue().c_str());
        ntp->setServer2(ntpServer2->getValue().c_str());
        ntp->setServer3(ntpServer3->getValue().c_str());
        ntp->setOffsetGmt(ntpOffsetGmt->getValue());
        ntp->setOffsetDst(ntpOffsetDst->getValue());
    } else {
        ntp->setServer1("");
        ntp->setServer2("");
        ntp->setServer3("");
        ntp->setOffsetGmt(0);
        ntp->setOffsetDst(0);
    }
}

NtpForm *_ntpForm = nullptr;

NtpForm *getNtpForm() {
    if (!_ntpForm) {
        _ntpForm = new NtpForm();
    }
    return _ntpForm;
}