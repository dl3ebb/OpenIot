#include "RebootForm.h"
#include <Connector/WebConnector.h>
#include <OpenIot.h>
#include <nvs_flash.h>

RebootForm::RebootForm() : WebForm() {

    reboot = webConnector->registerElement(new ButtonElement("_SYS_REBOOT_REBOOT"), this);
    reset = webConnector->registerElement(new ButtonElement("_SYS_REBOOT_FACTORY_RESET"), this);

    eventManager.addListener<WebButtonClickedEvent>(WebButtonClicked, "_SYS_REBOOT_REBOOT", [this](WebButtonClickedEvent *event) {
        logger->info(F("Reboot Requested"));
        openIot.rebootDevice();
    });

    eventManager.addListener<WebButtonClickedEvent>(WebButtonClicked, "_SYS_REBOOT_FACTORY_RESET", [this](WebButtonClickedEvent *event) {
        logger->info(F("Factory Reset Clicked"));
        nvs_flash_erase();
        nvs_flash_init();
        openIot.rebootDevice();
    });
}

RebootForm *_rebootForm = nullptr;

RebootForm *getRebootForm() {
    if (!_rebootForm) {
        _rebootForm = new RebootForm();
    }
    return _rebootForm;
}
