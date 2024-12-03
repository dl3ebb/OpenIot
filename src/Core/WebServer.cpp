#include "WebServer.h"
#include <Modules/Wifi.h>


AsyncWebServer *_webServer = nullptr;

AsyncWebServer *getWebServer() {
    if (!_webServer) {
        _webServer = new AsyncWebServer(80);
    }
    return _webServer;
}
