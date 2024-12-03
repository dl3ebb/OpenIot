#include "WebUpdateForm.h"
#include <Connector/MqttConnector.h>
#include <Connector/WebConnector.h>
#include <Modules/WebUpdate.h>
#include <OpenIot.h>

WebUpdateForm::WebUpdateForm() : WebForm() {
    enableUpdate = webConnector->registerElement(new BoolElement("_SYS_UPDATE_ENABLE"), this);
    server = webConnector->registerElement(new StringElement("_SYS_UPDATE_SERVER"), this);
    autoUpdate = webConnector->registerElement(new BoolElement("_SYS_AUTO_UPDATE"), this);
    save = webConnector->registerElement(new ButtonElement("_SYS_UPDATE_SAVE"), this);
    software = webConnector->registerElement(new StringElement("_SYS_UPDATE_SOFTWARE"), this);
    installedVersion = webConnector->registerElement(new StringElement("_SYS_UPDATE_VERSION_INSTALLED"), this);
    availableVersion = webConnector->registerElement(new StringElement("_SYS_UPDATE_VERSION_AVAILABLE"), this);
    doUpdateButton = webConnector->registerElement(new ButtonElement("_SYS_UPDATE_UPDATE"), this);
    progressDialog = webConnector->registerElement(new ProgressDialogElement("_SYS_UPDATE_PROGRESS"), this);

    this->setSubmitButton(save);

    eventManager.addListener<WebUpdateVersionEvent>(WebUpdateVersion, [this](WebUpdateVersionEvent *event) {
        software->setValue(String(event->appName));
        installedVersion->setValue(String(event->installedVersion));
        availableVersion->setValue(String(event->avaliableVersion));
        if (event->installedVersion < event->avaliableVersion) {
            doUpdateButton->enable();
        } else {
            doUpdateButton->disable();
        }
    });

    eventManager.addListener<WebButtonClickedEvent>(WebButtonClicked, "_SYS_UPDATE_UPDATE", [this](WebButtonClickedEvent *event) {
        eventManager.fireEvent(new WebUpdatePerformUpdateEvent());
    });

    eventManager.addListener<WebUpdateStartEvent>(WebUpdateStart, [this](WebUpdateStartEvent *event) {
        progressDialog->show();
    });

    eventManager.addListener<WebUpdateEndEvent>(WebUpdateEnd, [this](WebUpdateEndEvent *event) {
        progressDialog->hide();
    });

    eventManager.addListener<WebUpdateLabelEvent>(WebUpdateLabel, [this](WebUpdateLabelEvent *event) {
        progressDialog->setLabel(event->label);
    });

    eventManager.addListener<WebUpdateTotalLengthEvent>(WebUpdateTotalLength, [this](WebUpdateTotalLengthEvent *event) {
        progressDialog->setMaxValue(event->totalLength);
    });

    eventManager.addListener<WebUpdateCurrentLengthEvent>(WebUpdateCurrentLength, [this](WebUpdateCurrentLengthEvent *event) {
        progressDialog->setCurrentValue(event->currentLength);
    });
}

bool WebUpdateForm::checkFormData() {
    bool hasError = false;

    if (enableUpdate->getValue()) {
        if (server->getValue() == "") {
            server->setError(F("Server may not be empty"));
            hasError = true;
        } else {
            server->setError("");
        }
    } else {
        server->setError("");
    }

    if (hasError) {
        save->disable();
        return false;
    } else {
        save->enable();
        return true;
    }
}

void WebUpdateForm::setup() {
    enableUpdate->setValue(webUpdate->getEnable());
    server->setValue(webUpdate->getServer());
    autoUpdate->setValue(webUpdate->getAutoUpdate());
    software->setValue(openIot.getApplicationName());
    installedVersion->setValue(String(openIot.getApplicationBuild()));
    availableVersion->setValue(String(openIot.getApplicationBuild()));
    doUpdateButton->disable();
}

void WebUpdateForm::submit() {
    webUpdate->setEnable(enableUpdate->getValue());
    if (webUpdate->getEnable()) {
        webUpdate->setServer(server->getValue().c_str());
        webUpdate->setAutoUpdate(autoUpdate->getValue());
    } else {
        webUpdate->setServer("");
        webUpdate->setAutoUpdate(false);
    }
}

WebUpdateForm *_webUpdateForm = nullptr;

WebUpdateForm *getWebUpdateForm() {
    if (!_webUpdateForm) {
        _webUpdateForm = new WebUpdateForm();
    }
    return _webUpdateForm;
}
