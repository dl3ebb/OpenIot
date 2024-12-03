#include "WebCountdownElement.h"
#include <Connector/WebConnector.h>

void WebCountdownElement::setup() {
}

void WebCountdownElement::toJsonObject(JsonObject obj) {
    obj["started"] = started;
}

void WebCountdownElement::start() {
    if (!started) {
        started = true;
        webConnector->sendElement(this);
    }
}
