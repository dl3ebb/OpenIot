#include "DeviceForm.h"
#include <Connector/WebConnector.h>
#include <Core/AppHostname.h>

DeviceForm::DeviceForm() : WebForm() {
    deviceName = webConnector->registerElement(new StringElement("_SYS_DEVICE_NAME"), this);
    save = webConnector->registerElement(new ButtonElement("_SYS_DEVICE_SAVE"), this);
    setSubmitButton(save);
}

bool DeviceForm::checkFormData() {
    bool hasError = false;

    if (deviceName->getValue() == "") {
        deviceName->setError(F("DeviceName may not be empty."));
        hasError = true;
    } else {
        deviceName->setError(F(""));
    }

    if (hasError) {
        save->disable();
        return false;
    } else {
        save->enable();
        return true;
    }
}

void DeviceForm::setup() {
    deviceName->setValue(appHostname->getHostname());
    checkFormData();
}

void DeviceForm::submit() {
    appHostname->sethostname(deviceName->getValue());
}

DeviceForm *_deviceForm = nullptr;

DeviceForm *getDeviceForm() {
    if (!_deviceForm) {
        _deviceForm = new DeviceForm();
    }
    return _deviceForm;
}
