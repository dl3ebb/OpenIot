#include "Wifi.h"
#include <OpenIot.h>
#include <Core/EventManager.h>
#include <ESP32Ping.h>
#include <Logger/Logger.h>
#include <Ticker.h>
#include <WiFi.h>

/*
 * The Access Point mode triggers a ticker every second. After 5 Minutes the device will restart
 **/

Ticker accessPointModeTicker;
int resetCounter = 0;
void resetApMode() {
    resetCounter++;
    if (resetCounter == 300) {
        logger->debug(F("Accesspoint mode timeout. -> Reset"));
        ESP.restart();
    }
    eventManager.fireEvent(new AccessPointTimeoutTickerEvent());
}

/**
 * This is used to monitor the current wifi connection. If it fades away, the device is reset.
 */
Ticker wifiMonitorTicker;
void wifiMonitor() {
    wifi->monitorWifi();
}

void Wifi::monitorWifi() {
    if (Ping.ping(gatewayIp, 1)) {
        monitorErrorCounter = 0;
    } else {

        monitorErrorCounter++;

        if (monitorErrorCounter >= 5) {
            logger->warn(F("Gateway not reachable for 25secs. ->Reset"));
            ESP.restart();
        }
    }
}

Wifi::Wifi() : Module(), networkStatus(NetworkStatus::NONE) {
    openIot.registerModule(this);
}

void Wifi::setup() {
    scanWifi();
    setupWifi();
    logSettings();
}

/**
 * Scan for available Wifi Networks.
 *
 */
void Wifi::scanWifi() {
    logger->debug(F("Scanning Wifi"));
    networkMap.clear();
    int n = WiFi.scanNetworks();
    logger->debug(F("Found %d networks"), n);

    for (int i = 0; i < n; ++i) {
        String ssid = WiFi.SSID(i);
        String keySsid = ssid;
        keySsid.toLowerCase();
        int32_t rssi = WiFi.RSSI(i);
        uint8_t *bssid = WiFi.BSSID(i);

        //-- For duplicate SSIDs save the stronger bssid -------------
        if (networkMap.find(keySsid) == networkMap.end() || networkMap[keySsid].rssi < rssi) {
            networkMap[keySsid].ssid = ssid;
            networkMap[keySsid].rssi = rssi;
            networkMap[keySsid].bssid = bssid;
        }
    }
    WiFi.scanDelete();
    //--- Dump the available networt to the console -----------------
    for (const auto &pair : networkMap) {
        logger->debug(F("Found : %s %ld "), pair.second.ssid.c_str(), pair.second.rssi);
    }
}

/**
 * Setup the Wifi Network
 * If both Wifi networks are enabled, the device will connect to the strongest signal.
 */
void Wifi::setupWifi() {
    logger->debug(F("Set up wifi connection"));
    WiFi.enableSTA(true);
    WiFi.setHostname(appHostname->getHostname().c_str());

    String bestSSID = "";
    uint8_t *bestBSSID;
    String bestPassword = "";

    unsigned long startAttemptTime = millis();
    String keySsid1 = ssid1->getValue();
    keySsid1.toLowerCase();
    String keySsid2 = ssid2->getValue();
    keySsid2.toLowerCase();

    // If both ssid's are enabled and could be reached, select the stronger one.
    if (enablePrimary && networkMap.find(keySsid1) != networkMap.end() &&
        enableSecondary && networkMap.find(keySsid2) != networkMap.end()) {
        if (networkMap[keySsid1].rssi > networkMap[keySsid2].rssi) {
            bestSSID = networkMap[keySsid1].ssid;
            bestBSSID = networkMap[keySsid1].bssid;
            bestPassword = password1->getValue();
        } else {
            bestSSID = networkMap[keySsid2].ssid;
            bestBSSID = networkMap[keySsid2].bssid;
            bestPassword = password2->getValue();
        }
    } else if (enablePrimary && networkMap.find(keySsid1) != networkMap.end()) {
        bestSSID = networkMap[keySsid1].ssid;
        bestBSSID = networkMap[keySsid1].bssid;
        bestPassword = password1->getValue();
    } else if (enableSecondary && networkMap.find(keySsid2) != networkMap.end()) {
        bestSSID = networkMap[keySsid2].ssid;
        bestBSSID = networkMap[keySsid2].bssid;
        bestPassword = password2->getValue();
    }

    //--- Now bestxxx should contain the connection information ----------------
    if (bestSSID != "") {
        //--- Connect to the selected network -------------------------
        logger->debug(F("Connecting to %s - pw : %s "), bestSSID.c_str(), bestPassword.c_str());

        WiFi.begin(bestSSID.c_str(), bestPassword.c_str(), 0, bestBSSID);
        unsigned long startAttemptTime = millis();

        while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
            delay(100);
        }

        if (WiFi.status() == WL_CONNECTED) {
            logger->debug(F("Connected to %s"), bestSSID.c_str());
            networkStatus = NetworkStatus::STATION;
            IPAddress ip = WiFi.gatewayIP();
            sprintf(gatewayIp, "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
            wifiMonitorTicker.attach(5, ::wifiMonitor);
            eventManager.fireEvent(new WifiConnectedEvent());
        } else {
            logger->debug(F("Failed to connect to %s"), bestSSID.c_str());
            startAccessPoint();
        }
    } else {
        logger->debug(F("None of the preferred SSIDs are available."));
        startAccessPoint();
    }
}

void Wifi::logSettings() {
    logger->info(F("---- Wifi settings : ------"));
    logger->info(F("Accesspoint SSID      : %s"), apSsid->getValue().c_str());
    logger->info(F("Accesspoint Password  : %s"), apPassword->getValue().c_str());
    logger->info(F("Accesspoint Open      : %s"), (apOpen->getValue() ? "true" : "false"));

    logger->info(F("Wifi Primary Enabled  : %s"), (enablePrimary->getValue() ? "true" : "false"));
    logger->info(F("Primary ssid          : %s"), ssid1->getValue().c_str());
    logger->info(F("Primary Password      : %s"), password1->getValue().c_str());
    logger->info(F("Wifi Seconday Enabled : %s"), (enableSecondary->getValue() ? "true" : "false"));
    logger->info(F("Secondary ssid        : %s"), ssid2->getValue().c_str());
    logger->info(F("Secondary Password    : %s"), password2->getValue().c_str());

    logger->info(F("---- Wifi status : ------"));
    if (networkStatus == NetworkStatus::STATION) {
        logger->info(F("Network               : Station Mode"));
        logger->info(F("SSID                  : %s "), WiFi.SSID().c_str());
        logger->info(F("RSSI                  : %d "), WiFi.RSSI());
        logger->info(F("Channel               : %d "), WiFi.channel());
        IPAddress ip = WiFi.localIP();
        logger->info(F("IP address            : %u.%u.%u.%u "), ip[0], ip[1], ip[2], ip[3]);
        ip = WiFi.gatewayIP();
        logger->info(F("Gateway IP address    : %u.%u.%u.%u "), ip[0], ip[1], ip[2], ip[3]);
    } else {
        logger->info(F("Network               : Accesspoint"));
        IPAddress ip = WiFi.softAPIP();
        logger->info(F("IP address            : %u.%u.%u.%u "), ip[0], ip[1], ip[2], ip[3]);
    }
}

/**
 * Start the fallback accesspoint
 */
void Wifi::startAccessPoint() {
    logger->debug(F("No network found ! Starting Fallback AP "));
    if (!WiFi.softAP(apSsid->getValue(), apPassword->getValue())) {
        logger->debug(F("Soft AP creation failed."));
        ESP.restart();
    }
    IPAddress ip = WiFi.softAPIP();
    logger->debug(F("Accesspoint ready. IP address:  %u.%u.%u.%u "), ip[0], ip[1], ip[2], ip[3]);
    networkStatus = NetworkStatus::ACCESSPOINT;
    eventManager.fireEvent(new AccessPointStartedEvent());
    accessPointModeTicker.attach(1, ::resetApMode);
}

Wifi *_wifi = nullptr;

Wifi *getWifi() {
    if (!_wifi) {
        _wifi = new Wifi();
    }
    return _wifi;
}
