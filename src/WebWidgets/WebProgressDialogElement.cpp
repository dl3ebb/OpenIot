#include "WebProgressDialogElement.h"
#include <Connector/WebConnector.h>

void WebProgressDialogElement::setup() {
}

void WebProgressDialogElement::toJsonObject(JsonObject obj) {
    obj["visible"] = visible;
    obj["label"] = label;
    obj["percentage"] = percentage;
}

void WebProgressDialogElement::show() {
    if (!visible) {
        visible = true;
        webConnector->sendElement(this);
    }
}

void WebProgressDialogElement::hide() {
    if (visible) {
        visible = false;
        webConnector->sendElement(this);
    }
}

void WebProgressDialogElement::setLabel(String label_) {
    if (label != label_) {
        label = label_;
        webConnector->sendElement(this);
    }
}

void WebProgressDialogElement::setMaxValue(int maxValue_) {
    if (maxValue != maxValue_) {
        maxValue = maxValue_;
        if (maxValue != 0) {
            int newPercentage = currentValue * 100 / maxValue;
            if (newPercentage != percentage) {
                percentage = newPercentage;
                webConnector->sendElement(this);
            }
        }
    }
}

void WebProgressDialogElement::setCurrentValue(int currentValue_) {
    if (currentValue != currentValue_) {
        currentValue = currentValue_;
        if (maxValue != 0) {
            int newPercentage = currentValue * 100 / maxValue;
            if (newPercentage != percentage) {
                percentage = newPercentage;
                webConnector->sendElement(this);
            }
        }
    }
}
