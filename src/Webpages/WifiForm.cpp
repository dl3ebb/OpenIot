#include "WifiForm.h"
#include <Connector/WebConnector.h>
#include <Modules/Wifi.h>

WifiForm::WifiForm() : WebForm() {

    enablePrimary = webConnector->registerElement(new BoolElement("_SYS_WIFI_ENABLE_PRIMARY"), this);
    ssid1 = webConnector->registerElement(new SelectElement("_SYS_WIFI_SSID1"), this);
    ;
    password1 = webConnector->registerElement(new StringElement("_SYS_WIFI_PASSWORD1"), this);
    enableSecondary = webConnector->registerElement(new BoolElement("_SYS_WIFI_ENABLE_SECONDARY"), this);
    ssid2 = webConnector->registerElement(new SelectElement("_SYS_WIFI_SSID2"), this);
    ;
    password2 = webConnector->registerElement(new StringElement("_SYS_WIFI_PASSWORD2"), this);
    ;
    save = webConnector->registerElement(new ButtonElement("_SYS_WIFI_SUBMIT"), this);

    this->setSubmitButton(save);
}

bool WifiForm::checkFormData() {
    bool hasError = false;

    if (enablePrimary->getValue()) {
        if (ssid1->getValue() == "") {
            ssid1->setError(F("SSID may not be empty"));
            hasError = true;
        } else {
            ssid1->setError("");
        }
    }

    if (enableSecondary->getValue()) {
        if (ssid2->getValue() == "") {
            ssid2->setError(F("SSID may not be empty"));
            hasError = true;
        } else {
            ssid2->setError("");
        }
    }

    if (hasError) {
        save->disable();
        return false;
    } else {
        save->enable();
        return true;
    }
}

void WifiForm::setup() {
    std::vector<SelectboxOption> wifiNetworks;
    for (const auto &network : wifi->getNnetworkMap()) {
        SelectboxOption option;
        option.value = network.second.ssid;
        option.text = network.second.ssid + "(" + network.second.rssi + " dbm)";
        wifiNetworks.push_back(option);
    }

    enablePrimary->setValue(wifi->getEnablePrimary());
    ssid1->setValue(wifi->getSsid1());
    ssid1->setOptions(wifiNetworks);
    password1->setValue(wifi->getPassword1());

    enableSecondary->setValue(wifi->getEnableSecondary());
    ssid2->setValue(wifi->getSsid2());
    ssid2->setOptions(wifiNetworks);
    password2->setValue(wifi->getPassword2());
}

void WifiForm::submit() {
    wifi->setEnablePrimary(enablePrimary->getValue());
    if (wifi->getEnablePrimary()) {
        wifi->setSsid1(ssid1->getValue().c_str());
        wifi->setPassword1(password1->getValue().c_str());
    } else {
        wifi->setSsid1("");
        wifi->setPassword1("");
    }

    wifi->setEnableSecondary(enableSecondary->getValue());
    if (wifi->getEnableSecondary()) {
        wifi->setSsid2(ssid2->getValue().c_str());
        wifi->setPassword2(password2->getValue().c_str());
    } else {
        wifi->setSsid2("");
        wifi->setPassword2("");
    }
    logger->debug(F("Wifi Submit"));
}

WifiForm *_wifiForm = nullptr;

WifiForm *getWifiForm() {
    if (!_wifiForm) {
        _wifiForm = new WifiForm();
    }
    return _wifiForm;
}
