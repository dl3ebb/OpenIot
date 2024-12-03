#ifndef CONFIG_H
#define CONFIG_H
#include <Core/EventManager.h>
#include <Preferences.h>

/**
 * The config elements are used to store configuration values to the nvs ram of teh device 
 * 
 * Each element has a group and a topic. If "restartNeeded" is set to true, a restart is needed if the value changes.
 * The ConfigElement will fire a RestartNeeded Event, that will be catched by the web application to show a Lable where the user may click on. 
 */
class ConfigBase {
  protected:
    const char *group;
    const char *property;
    bool restartNeeded;

    ConfigBase(const char *group_, const char *property_, bool restartNeeded_ = false) : group(group_), property(property_), restartNeeded(restartNeeded_) {}
};

class RestartNeededEvent : public Event {
  public:
    RestartNeededEvent() : Event(RestartNeeded, "") {}
    void cleanup() override { delete this; }
    String toString() override { return "RestartNeededEvent"; }
};

class ConfigBool : public ConfigBase {
  private:
    bool value;

  public:
    ConfigBool(const char *group, const char *property, bool defaultValue_ = false, bool restartNeeded_ = false) : ConfigBase(group, property, restartNeeded_) {
        Preferences prefs;
        prefs.begin(group);
        value = prefs.getBool(property, defaultValue_);
        prefs.end();
    }
    bool getValue() { return value; };
    void setValue(bool value_) {
        if (value != value_) {
            value = value_;
            Preferences prefs;
            prefs.begin(group);
            prefs.putBool(property, value);
            prefs.end();
            if (restartNeeded) {
                eventManager.fireEvent(new RestartNeededEvent());
            }
        }
    }
};

class ConfigString : public ConfigBase {
  private:
    String value;

  public:
    ConfigString(const char *group, const char *property, String defaultValue_ = String(), bool restartNeeded_ = false) : ConfigBase(group, property, restartNeeded_) {
        Preferences prefs;
        prefs.begin(group);
        value = prefs.getString(property, defaultValue_);
        prefs.end();
        /*
        Serial.print ("Group ");
        Serial.print (group);
        Serial.print (" - property ");
        Serial.print (property);
        Serial.print (" - Val ");
        Serial.println (value);
        */
    }
    String getValue() { return value; };
    void setValue(String value_) {
        if (value != value_) {
            value = value_;
            Preferences prefs;
            prefs.begin(group);
            prefs.putString(property, value);
            prefs.end();
            if (restartNeeded) {
                eventManager.fireEvent(new RestartNeededEvent());
            }
        }
    }
};

class ConfigLong : public ConfigBase {
  private:
    long value;

  public:
    ConfigLong(const char *group, const char *property, long defaultValue_ = 0, bool restartNeeded_ = false) : ConfigBase(group, property, restartNeeded_) {
        Preferences prefs;
        prefs.begin(group);
        value = prefs.getLong(property, defaultValue_);
        prefs.end();
    }
    long getValue() { return value; };
    void setValue(long value_) {
        if (value != value_) {
            value = value_;
            Preferences prefs;
            prefs.begin(group);
            prefs.putLong(property, value);
            prefs.end();
            if (restartNeeded) {
                eventManager.fireEvent(new RestartNeededEvent());
            }
        }
    }
};

#endif