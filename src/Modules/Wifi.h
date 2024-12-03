#ifndef WIFI_H
#define WIFI_H
/**
 * Wifi provides the network connectivity for the device.
 *
 * After startup, the device scans the available networks. If one of the two networks that can be configured is found,
 * the device will try to connect to the stronger of both.
 *
 * If there are noch networks configured or the connection fails a fallback accesspoint is opened for thge initial configuration.
 *
 * If nothing happends during the next 5 Minutes, the device is restarted. This may solve the situation that after a power outage the
 * device will restart faster as the hotspots that it should connect to.
 */
#include "Module.h"
#include <Arduino.h>
#include <Core/AppHostname.h>
#include <Core/Config/Config.h>
#include <map>

enum class NetworkStatus {
    NONE,
    STATION,
    ACCESSPOINT
};

/**
 * Attributes from the network scan
 */
struct WifiNetwork {
    String ssid; // WiFi uses Strings as well.
    uint8_t *bssid;
    int32_t rssi;
};

class Wifi : public Module {
  private:
    ConfigString *apSsid = new ConfigString("OI/Wifi", "ap_ssid", appHostname->getHostname(), true);
    ConfigString *apPassword = new ConfigString("OI/Wifi", "ap_password", "", true);
    ConfigBool *apOpen = new ConfigBool("OI/Wifi", "ap_open", true, true);

    ConfigBool *enablePrimary = new ConfigBool("OI/Wifi", "en_prim", false, true);
    ConfigString *ssid1 = new ConfigString("OI/Wifi", "ssid1", "", true);
    ConfigString *password1 = new ConfigString("OI/Wifi", "password1", "", true);

    ConfigBool *enableSecondary = new ConfigBool("OI/Wifi", "en_sec", false, true);
    ConfigString *ssid2 = new ConfigString("OI/Wifi", "ssid2", "", true);
    ConfigString *password2 = new ConfigString("OI/Wifi", "password2", "", true);

    NetworkStatus networkStatus;

    char gatewayIp[20];
    int monitorErrorCounter = 0;

    std::map<String, WifiNetwork> networkMap;

  public:
    Wifi();
    void setup();
    void loop(bool firstLoop) {};

    void scanWifi();
    void setupWifi();
    void startAccessPoint();
    void monitorWifi();

    NetworkStatus getNetworkStatus() { return networkStatus; }
    std::map<String, WifiNetwork> getNnetworkMap() { return networkMap; };

    //--- Configuration --------------------------------
    void setApSsid(const char *apSsid_) { apSsid->setValue(apSsid_); }
    void setApPassword(const char *apPassword_) { apPassword->setValue(apPassword_); }
    void setApOpen(bool apOpen_) { apOpen->setValue(apOpen_); }

    void setEnablePrimary(bool enablePrimary_) { enablePrimary->setValue(enablePrimary_); }
    void setSsid1(const char *ssid1_) { ssid1->setValue(ssid1_); }
    void setPassword1(const char *password1_) { password1->setValue(password1_); }

    void setEnableSecondary(bool enableSecondary_) { enableSecondary->setValue(enableSecondary_); }
    void setSsid2(const char *ssid2_) { ssid2->setValue(ssid2_); }
    void setPassword2(const char *password2_) { password2->setValue(password2_); }

    String getapSsid() { return apSsid->getValue(); }
    String getapPassword() { return apPassword->getValue(); }
    bool getApOpen() { return apOpen->getValue(); }

    bool getEnablePrimary() { return enablePrimary->getValue(); }
    String getSsid1() { return ssid1->getValue(); }
    String getPassword1() { return password1->getValue(); }

    bool getEnableSecondary() { return enableSecondary->getValue(); }
    String getSsid2() { return ssid2->getValue(); }
    String getPassword2() { return password2->getValue(); }

  private:
    void logSettings();
};

class WifiConnectedEvent : public Event {
  public:
    WifiConnectedEvent() : Event(WifiConnected, "") {}
    void cleanup() override { delete this; }
    String toString() override { return "WifiConnectedEvent"; }
};

class WifiDisconnectedEvent : public Event {
  public:
    WifiDisconnectedEvent() : Event(WifiDisconnected, "") {}
    void cleanup() override { delete this; }
    String toString() override { return "WifiDisconnectedEvent"; }
};

class AccessPointStartedEvent : public Event {
  public:
    AccessPointStartedEvent() : Event(AccesspointStarted, "") {}
    void cleanup() override { delete this; }
    String toString() override { return "AccessPointStartedEvent"; }
};

class AccessPointTimeoutTickerEvent : public Event {
  public:
    AccessPointTimeoutTickerEvent() : Event(AccessPointTimeoutTicker, "") {}
    void cleanup() override { delete this; }
    String toString() override { return "AccessPointTimeoutTickerEvent"; }
};

#define wifi getWifi()

Wifi *getWifi();

#endif