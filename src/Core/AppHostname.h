#ifndef APP_HOSTNAME_H
#define APP_HOSTNAME_H
#include <Arduino.h>
#include <Core/Config/Config.h>

/**
 * The appHostname is a small utility class that provides a unique hostname for the device.
 * It is used for the AP SSID or the DSN hostname of the device. It may be overridden by setting a hostname, that is stored in the nvs ram
 */
class AppHostname {
    ConfigString *cfgAppHostname;

  public:
    AppHostname();
    void sethostname(String _appHostname) { cfgAppHostname->setValue(_appHostname); }
    String getHostname() { return cfgAppHostname->getValue(); };
};

#define appHostname getAppHostname()

AppHostname *getAppHostname();

#endif