#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <Logger/Logger.h>
#include <functional>
#include <map>
#include <memory>
#include <queue>
#include <vector>

/**
 * The complete communication between the different parts of the application is based on Events.
 * Events are managed by the Eventmanager. A module may register a listener for a specific event
 * on the eventmanager or fire an event on the eventmanager.
 *
 * This design decouples the application structure and makes it easier to extend and maintain the application.
 *
 * An Event may have a destination. This usually is an element id, but may be any string. The eventlistener m
 * ay set an destination string as well. If set, the eventmanager will filter out the Events on that destination.
 */
class Event {
  public:
    int type;
    String destination;
    Event(int type_, String destination_) : type(type_), destination(destination_) {}
    virtual ~Event() = default;

    virtual void cleanup() = 0;
    virtual String toString() = 0;
};

template <typename T>
using TypedEventHandler = std::function<void(T *)>;

using EventHandler = std::function<void(Event *)>;

class EventManager {
  private:
    std::queue<Event *> highPriorityQueue;
    std::queue<Event *> lowPriorityQueue;

    std::map<int, std::vector<std::pair<String, EventHandler>>> listeners;

  public:
    template <typename T>
    void addListener(int type, String destination, TypedEventHandler<T> handler) {
        listeners[type].push_back({destination, [handler](Event *event) {
                                       handler(static_cast<T *>(event));
                                   }});
    }

    template <typename T>
    void addListener(int type, TypedEventHandler<T> handler) {
        listeners[type].push_back({"", [handler](Event *event) {
                                       handler(static_cast<T *>(event));
                                   }});
    }

    /**
     * During startup and configuration there may be some unneccessary events in the queues. We need to delete tham 
     * before the application is started
     */
    void startEventManager() {
        while (!highPriorityQueue.empty()) {
            highPriorityQueue.pop();
        }
        while (!lowPriorityQueue.empty()) {
            lowPriorityQueue.pop();
        }
    }

    void fireEvent(Event *event, bool highPriority = false) {
        if (highPriority) {
            highPriorityQueue.push(event);
        } else {
            lowPriorityQueue.push(event);
        }
    }

    /**
     * This is called by the openiot core tor process one event in each loop. 
     */
    void processEvents() {
        if (!highPriorityQueue.empty()) {
            Event *event = highPriorityQueue.front();
            highPriorityQueue.pop();
            dispatchEvent(event);
        } else if (!lowPriorityQueue.empty()) {
            Event *event = lowPriorityQueue.front();
            lowPriorityQueue.pop();
            dispatchEvent(event);
        }
    }

    /**
     * This is can be called after an event is fired to make sure all events are processed.
     * It is mainly used by the web update to make sure the web application is updated by the progress of the update.
     */
    void processAllEvents() {
        while (!highPriorityQueue.empty()) {
            Event *event = highPriorityQueue.front();
            highPriorityQueue.pop();
            dispatchEvent(event);
        }
        while (!lowPriorityQueue.empty()) {
            Event *event = lowPriorityQueue.front();
            lowPriorityQueue.pop();
            dispatchEvent(event);
        }
    }

  private:
    void dispatchEvent(Event *event) {
        logger->debug(F("EventManager.dispatch : %s "), event->toString().c_str());
        auto it = listeners.find(event->type);
        if (it != listeners.end()) {
            for (const auto &pair1 : it->second) {
                if (pair1.first.isEmpty() || pair1.first == event->destination) {
                    pair1.second(event);
                }
            }
        }
        event->cleanup();
    }
};

extern EventManager eventManager;

/**
 * This enumerates the System Event Types. This speeds up the Event 
 * procesing by compariong to an int instead of an event class name.
 */
enum SystemEventTypes {

    RestartNeeded = 32000,
    StartRebootCountdown,

    //---- Wifi Events ------------------
    WifiDisconnected,
    WifiConnected,
    AccesspointStarted,
    AccessPointTimeoutTicker,

    //--- MQTT --------------------------
    MqttConnected,

    NewBoolValue,
    NewLongValue,
    NewStringValue,

    //--- Webserver ----------------------
    WebClientConnected,

    WebButtonClicked,

    //--- WebUpdate -----------------------
    WebUpdateStart,
    WebUpdateEnd,
    WebUpdateLabel,
    WebUpdateTotalLength,
    WebUpdateCurrentLength,
    WebUpdateVersion,
    WebUpdatePerformUpdate,

};

class StartRebootCountdownEvent : public Event {
  public:
    StartRebootCountdownEvent() : Event(StartRebootCountdown, "") {}
    void cleanup() override { delete this; }
    String toString() override { return "StartRebootCountdown"; }
};

/**
 * The core Events for setting new Values 
 */
class NewBoolValueEvent : public Event {
  public:
    bool newValue;
    String toStringResult;

    NewBoolValueEvent(String destination_, bool newValue_) : Event(NewBoolValue, destination_), newValue(newValue_) {}
    void cleanup() override {
        delete (this);
    }

    String toString() override {
        if (toStringResult.isEmpty()) {
            toStringResult = "NewBoolValueEvent (";
            toStringResult += type;
            toStringResult += ") source : ";
            toStringResult += destination;
            toStringResult += " - ";
            toStringResult += (newValue ? "true" : "false");
        }
        return toStringResult.c_str();
    }
};

class NewLongValueEvent : public Event {
  public:
    long newValue;
    String toStringResult;

    NewLongValueEvent(String destination_, long newValue_) : Event(NewLongValue, destination_), newValue(newValue_) {}
    void cleanup() override {
        delete (this);
    }

    String toString() override {
        if (toStringResult.isEmpty()) {
            toStringResult = "NewLongValueEvent (";
            toStringResult += type;
            toStringResult += ") source : ";
            toStringResult += destination;
            toStringResult += " - ";
            toStringResult += String(newValue);
        }
        return toStringResult;
    }
};

class NewStringValueEvent : public Event {
  public:
    String newValue;
    String toStringResult;

    NewStringValueEvent(String destination_, String newValue_) : Event(NewStringValue, destination_), newValue(newValue_) {}
    void cleanup() override {
        delete (this);
    }

    String toString() override {
        if (toStringResult.isEmpty()) {
            toStringResult = "NewStringValueEvent (";
            toStringResult += type;
            toStringResult += ") source : ";
            toStringResult += destination;
            toStringResult += " - ";
            toStringResult += newValue;
        }
        return toStringResult;
    }
};

#endif