#ifndef WEB_UPDATE_H
#define WEB_UPDATE_H
#include "Module.h"
#include <Core/Config/Config.h>

/**
 * The webupdate Module allows update of the application from a central update server. if autoupdate is set to true, the
 * update is started as soon as a new version is found
 */
class WebUpdate : public Module {
    ConfigBool *enable = new ConfigBool("OI/Update", "enable", false, true);
    ConfigString *server = new ConfigString("OI/Update", "server", "", true);
    ConfigBool *autoUpdate = new ConfigBool("OI/Update", "autoUpdate", false, true);
    bool updateInProgress = false;

  public:
    WebUpdate();
    void setup();
    void loop(bool firstLoop) {};

    void updateCheck();
    bool checkForUpdate();
    int updateVersion = 0;
    int lastUpdateVersion = -1;
    bool performUpdate(bool updateFlash);

    //--- Config ----------------------------------------
    void setAutoUpdate(bool autoUpdate_) { autoUpdate->setValue(autoUpdate_); }
    void setServer(String server_) { server->setValue(server_); }
    void setEnable(bool enable_) { enable->setValue(enable_); }

    bool getAutoUpdate() { return autoUpdate->getValue(); }
    String getServer() { return server->getValue(); }
    bool getEnable() { return enable->getValue(); }

    void logSettings();
};

/**
 * These events are fired during the update process. An user interface may use these to show something to the user. 
 */
class WebUpdateStartEvent : public Event {
  public:
    WebUpdateStartEvent() : Event(WebUpdateStart, "") {}
    void cleanup() override { delete this; }
    String toString() override { return "WebUpdateStartEvent"; }
};

class WebUpdateEndEvent : public Event {
  public:
    WebUpdateEndEvent() : Event(WebUpdateEnd, "") {}
    void cleanup() override { delete this; }
    String toString() override { return "WebUpdateEndEvent"; }
};

class WebUpdateLabelEvent : public Event {
  public:
    String label;
    WebUpdateLabelEvent(String label_) : Event(WebUpdateLabel, ""), label(label_) {}
    void cleanup() override { delete this; }
    String toString() override { return "WebUpdateEndEvent"; }
};

class WebUpdateTotalLengthEvent : public Event {
  public:
    int totalLength;
    WebUpdateTotalLengthEvent(int totalLength_) : Event(WebUpdateTotalLength, ""), totalLength(totalLength_) {}
    void cleanup() override { delete this; }
    String toString() override { return "WebUpdateTotalLengthEvent : " + String(totalLength); }
};

class WebUpdateCurrentLengthEvent : public Event {
  public:
    int currentLength;
    WebUpdateCurrentLengthEvent(int currentLength_) : Event(WebUpdateCurrentLength, ""), currentLength(currentLength_) {}
    void cleanup() override { delete this; }
    String toString() override { return "WebUpdateCurrentLengthEvent : " + String(currentLength); }
};

class WebUpdateVersionEvent : public Event {
  public:
    String appName;
    int installedVersion;
    int avaliableVersion;
    WebUpdateVersionEvent(String appName_, int installedVersion_, int availableVersion_) : Event(WebUpdateVersion, ""), appName(appName_), installedVersion(installedVersion_), avaliableVersion(availableVersion_) {}
    void cleanup() override { delete this; }
    String toString() override { return "WebUpdateVersionEvent : Name : " + String(appName) + " inst.: " + String(installedVersion) + " avail.: " + String(avaliableVersion); }
};

/**
 * This event should be fired to actually trigger the manual update 
 */
class WebUpdatePerformUpdateEvent : public Event {
  public:
    WebUpdatePerformUpdateEvent() : Event(WebUpdatePerformUpdate, "") {}
    void cleanup() override { delete this; }
    String toString() override { return "WebUpdatPerformUpdateEvent"; }
};

#define webUpdate getWebUpdate()

WebUpdate *getWebUpdate();

#endif