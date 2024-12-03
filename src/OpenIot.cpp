#include <OpenIot.h>
#include <Ticker.h>
#include <Logger/Logger.h>
#include <Core/EventManager.h>
#include <Connector/Connector.h>
#include <Core/Config/Config.h>

extern Logger* _logger;

void OpenIot::registerConnector (Connector* connector) {
    connectors.push_back(connector);
};

void OpenIot::registerModule (Module* module) {
    modules.push_back(module);
}

void OpenIot::registerForm (WebForm* form) {
    forms.push_back(form);
};

OpenIot::OpenIot () {
}

void OpenIot::setLogger (Logger* logger_) {
    _logger = logger_;
}

void OpenIot::setup (String applicationName_, int applicationBuild_) {
    applicationName = applicationName_;
    applicationBuild = applicationBuild_;

    eventManager.startEventManager();

    for (Module* mod : modules){
        mod->__setup();
    }

    for (WebForm* form : forms){
        form->__setup();
    }

    for (Connector* con : connectors){
        con->setup();
    }
}

void OpenIot::loop () {
    eventManager.processEvents();

    for (Module* mod : modules){
        mod->loop(firstLoop);
    }

    for (Module* mod : modules){
        mod->loop(firstLoop);
    }

    for (Connector* con : connectors){
        con->loop(firstLoop);
    }

    if (firstLoop) {
        firstLoop = false;
    } 

}

void OpenIot::rebootDevice () {
    logger->info(F("Reboot request received"));
    eventManager.fireEvent (new StartRebootCountdownEvent());
    eventManager.processAllEvents();
    delay(1000); // wait for the message to be delivered to the server

    ESP.restart();
}

OpenIot openIot;