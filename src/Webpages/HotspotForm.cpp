#include "HotspotForm.h"
#include <Connector/WebConnector.h>
#include <Modules/Wifi.h>

HotspotForm::HotspotForm() : WebForm() {
    ssid = webConnector->registerElement(new StringElement("_SYS_HOTSPOT_SSID"), this);
    password = webConnector->registerElement(new StringElement("_SYS_HOTSPOT_PASSWORD"), this);
    openAccessPoint = webConnector->registerElement(new BoolElement("_SYS_HOTSPOT_OPEN"), this);
    submitButton = webConnector->registerElement(new ButtonElement("_SYS_HOTSPOT_SUBMIT"), this);

    this->setSubmitButton(submitButton);
}

bool HotspotForm::checkFormData() {
    bool hasError = false;
    if (ssid->getValue() == "") {
        ssid->setError(F("SSID may not be empty"));
        hasError = true;
    } else {
        ssid->setError("");
    }

    if (!openAccessPoint->getValue()) {
        if (password->getValue().length() < 8 || password->getValue().length() > 63) {
            password->setError(F("Password needs to be between 8 and 63 characters"));
            hasError = true;
        } else {
            password->setError("");
        }
    } else {
        password->setError("");
    }

    if (hasError) {
        submitButton->disable();
        return false;
    } else {
        submitButton->enable();
        return true;
    }
}

void HotspotForm::submit() {
    wifi->setApOpen(this->openAccessPoint->getValue());
    wifi->setApSsid(this->ssid->getValue().c_str());
    wifi->setApPassword(this->password->getValue().c_str());
}

void HotspotForm::setup() {
    this->ssid->setValue(wifi->getapSsid());
    this->password->setValue(wifi->getapPassword());
    this->openAccessPoint->setValue(wifi->getApOpen());
}

HotspotForm *_hotspotForm = nullptr;

HotspotForm *getHotspotForm() {
    if (!_hotspotForm) {
        _hotspotForm = new HotspotForm();
    }
    return _hotspotForm;
}
