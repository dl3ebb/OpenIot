#ifndef WEBSERVER_H
#define WEBSERVER_HZ

#include <ESPAsyncWebServer.h>

#define webServer getWebServer()

AsyncWebServer *getWebServer();

#endif