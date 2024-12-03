#include "AppHostname.h"

AppHostname::AppHostname () {
    char defaultHostname[32];
    uint64_t chipid = ESP.getEfuseMac();
    uint16_t chip = (uint16_t)(chipid >> 32);
    snprintf(defaultHostname, 23, "OPEN-IOT-%04X%08X", chip, (uint32_t)chipid);
    cfgAppHostname = new ConfigString("OI/HostName","hostname", defaultHostname,true);
}

AppHostname* _appHostName = nullptr; 

AppHostname* getAppHostname() {
    if (!_appHostName) {
        _appHostName = new AppHostname();
    }
    return _appHostName;
}


