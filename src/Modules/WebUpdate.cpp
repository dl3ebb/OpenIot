#include "WebUpdate.h"
#include <Ticker.h>
#include <HTTPClient.h>
#include <Update.h>
#include "Wifi.h"
#include <OpenIot.h>
#include <AsyncDelay.h>
#include <esp_task_wdt.h>

Ticker webUpdateTicker;

void webUpdateCheck () {
    webUpdate->updateCheck();
}

void WebUpdate::setup () {
    webUpdateTicker.attach(60, ::webUpdateCheck);
    updateCheck();
    logSettings();
}

WebUpdate::WebUpdate() : Module (), 
    updateInProgress(false) {
    openIot.registerModule(this);
    /**
     * Register the eventlistener for the WebUpdatePerformUpdateEvent
     */        
    eventManager.addListener<WebUpdatePerformUpdateEvent> (WebUpdatePerformUpdate, [this] (WebUpdatePerformUpdateEvent* event) {
        if (!updateInProgress) {
            //--- lock against duplicate requests -------------------
            updateInProgress = true;
            logger->info(F("Started Webupdate !"));
            //--- Stop the timer ------------------------------------            
            webUpdateTicker.detach();

            //--- Notify the ui that we will start now --------------------------------
            eventManager.fireEvent (new WebUpdateStartEvent());
            eventManager.processAllEvents();

            /**
             * The update are two fiiles. One for the binary and one for the spiffs image
             * if both succeed, the update was ok. 
             */
            if (webUpdate->performUpdate(false) && webUpdate->performUpdate(true)) {
                logger->info(F("Update succeeded. Restart device."));
            } else {
                logger->info(F("Update failed. Restart device."));
            }

            //--- Notify the ui that we will end now --------------------------------
            eventManager.fireEvent (new WebUpdateEndEvent());
            eventManager.processAllEvents();
            delay(1000); // wait for the message to be delivered to the server
            //--- Reboot the device -------------------------------------------------
            
            openIot.rebootDevice();
        }
    });
}

/**
 * This method is called by the ticker every 60 seconds to check for an update 
 */
void WebUpdate::updateCheck () {
    if (getAutoUpdate() && wifi->getNetworkStatus() == NetworkStatus::STATION && checkForUpdate()) {
        eventManager.fireEvent (new WebUpdatePerformUpdateEvent ());
    }
}

/**
 * This calls the OpenIot Script on the update server that returns the highest veersion number for the application 
 * returns true if there is an update available
 */
bool WebUpdate::checkForUpdate () {
    if (enable->getValue() && server->getValue() != "" && wifi->getNetworkStatus() == NetworkStatus::STATION) {
        HTTPClient http;

        //--- Build the URL for the update script --------------------------------------
        String url = "http://" + server->getValue() + "/uq.php?app=" + openIot.getApplicationName();
        http.begin(url);
        int httpCode = http.GET();

        //--- Web call failed. Log the reason and exit ---------------------------------
        if (httpCode != HTTP_CODE_OK) {
            logger->warn(F("Check for Update. failed for %s. Code : %d"), url.c_str(), httpCode );
            http.end ();
            return false; 
        }

        //--- We got a response from the server -----------------------------------------
        String payload = http.getString();
        updateVersion = payload.toInt();
        http.end();

        //--- Only do something if something changed ------------------------------------
        if (lastUpdateVersion != updateVersion) {
            lastUpdateVersion = updateVersion;
            if (updateVersion <= openIot.getApplicationBuild()) {
                logger->debug(F("No update. Installed Version : %d Version on Update Server : %d"), openIot.getApplicationBuild(), updateVersion );
            } else {
                logger->debug(F("Update available. Installed Version : %d Version on Update Server : %d"), openIot.getApplicationBuild(), updateVersion );
            }
            eventManager.fireEvent (new WebUpdateVersionEvent (openIot.getApplicationName(), openIot.getApplicationBuild(), updateVersion));
        }
        return (updateVersion > openIot.getApplicationBuild());
    }
    return false;
}

/**
 * This is the actual update. For a complete update the method is called 
 * twice. Once for the Binary and once for the SPIFFS image.
 */
bool WebUpdate::performUpdate (bool updateFlash) {

    if (updateFlash) {
        eventManager.fireEvent (new WebUpdateLabelEvent (F("Downloading Binary.")));
        eventManager.processAllEvents();
    } else {
        eventManager.fireEvent (new WebUpdateLabelEvent (F("Downloading SPIFFS Image")));
        eventManager.processAllEvents();
    }
  
    logger->debug(F("Heap : %ld"), esp_get_free_heap_size());
    
    //--- Timeout for the download of one packet. --------------------------
    esp_task_wdt_init(10, true);

    int buffer = 32767;
    if (buffer > esp_get_free_heap_size()) {
        logger->warn(F("Not enough heap size. Update aborted"));
        return false;
    }

    //--- Build the URL --------------------------------
    String firmwareUrl = "http://" + server->getValue() + "/update/img/" + openIot.getApplicationName() + "_" + updateVersion + (updateFlash ? ".bin" : ".img");

    HTTPClient http;
    http.begin(firmwareUrl);
    int httpCode = http.GET();
    if (httpCode != 200) {
        logger->warn(F("Download of new firmware %s failed (Code %d)!"), firmwareUrl.c_str(), httpCode);
        http.end();
        return false;
    } 
    
    int totalLength = http.getSize();
    logger->info(F(" Updating from %s (Length = %ld bytes)"), firmwareUrl.c_str(), totalLength);
    eventManager.fireEvent (new WebUpdateCurrentLengthEvent (0));
    eventManager.fireEvent (new WebUpdateTotalLengthEvent (totalLength));
    eventManager.processAllEvents();

    Update.begin(UPDATE_SIZE_UNKNOWN, updateFlash ? U_FLASH : U_SPIFFS);
    int len = totalLength;
    int  currentLength = 0;
    uint8_t *buff = new uint8_t[buffer];
  
    WiFiClient *stream = http.getStreamPtr();

    //--- The absolute timeout for the update process (2 Minutes)---- 
    AsyncDelay updateAsyncDelay;
    updateAsyncDelay.start(120000, AsyncDelay::MILLIS);
    while (http.connected() && (len > 0 || len == -1)) {
        if (updateAsyncDelay.isExpired()) {
            http.end();
            logger->warn (F("Update failed (Timeout)!"));
            return false;
        }

        eventManager.fireEvent (new WebUpdateCurrentLengthEvent (currentLength));
        eventManager.processAllEvents();

        esp_task_wdt_reset();
        vTaskDelay(pdMS_TO_TICKS(100)); 

        size_t size = stream->available();
        if (size) {
            uint16_t c = stream->readBytes(buff, ((size > buffer) ? buffer : size));
            Update.write(buff, c);
            currentLength += c;

            if (len > 0)
                len -= c;

            if (currentLength >= totalLength) {
                http.end();

                eventManager.fireEvent(new WebUpdateLabelEvent(F("Writing flash !")));
                eventManager.processAllEvents();
                bool ok = Update.end(true);
                if (!ok) {
                    logger->warn(F("Update failed !"));
                    return false;
                }
                logger->info(F("Update ok !"));
                return true;
            }
        }
    }

    /**
     * Reset Task Timer to 1 Second
    */
    esp_task_wdt_init(1, true);
}

void WebUpdate::logSettings() {
    logger->info (F("---- WebUpdate settings : ------"));
    logger->info (F("Webupdate enabled     : %s"), (enable->getValue() ? "true" : "false"));
    logger->info (F("Update Server         : %s"), server->getValue().c_str());
    logger->info (F("Autoupdate enabled    : %s"), (autoUpdate->getValue() ? "true" : "false"));
}

WebUpdate* _webUpdate = nullptr; 

WebUpdate* getWebUpdate() {
    if (!_webUpdate) {
        _webUpdate = new WebUpdate();
    }
    return _webUpdate;
}

