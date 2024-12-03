#include "IndexPage.h"
#include <OpenIot.h>
#include <Core/Config/Config.h>
#include <Connector/WebConnector.h>

IndexPage::IndexPage() : WebForm() {

    rebootRequired = webConnector->registerElement (new StringElement ("_SYS_REBOOT_NEEDED"), nullptr);
    rebootCountdown = webConnector->registerElement(new CountdownElement ("_SYS_REBOOT_COUNTDOWN"), nullptr);

    eventManager.addListener<RestartNeededEvent> (RestartNeeded, [this](RestartNeededEvent* event) {
        logger->info(F("got reboot required"));
        rebootRequired->setValue (F("Reboot is required. Click here to reboot !"));
    });

    eventManager.addListener<WebButtonClickedEvent> (WebButtonClicked, "_SYS_REBOOT_NEEDED", [this](WebButtonClickedEvent* event) {
        openIot.rebootDevice();
    });

    eventManager.addListener<StartRebootCountdownEvent> (StartRebootCountdown, [this](StartRebootCountdownEvent* event) {
        logger->info (F("Start Reboot Countdown"));
        rebootCountdown->start();
    });
}

IndexPage *_indexPage = nullptr;

IndexPage *getIndexPage() {
    if (!_indexPage) {
        _indexPage = new IndexPage();
    }
    return _indexPage;
}
