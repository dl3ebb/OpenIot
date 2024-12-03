#ifndef NTP_H
#define NTP_H
#include "Module.h"
#include <Core/Config/Config.h>

/**
 * The Module NTP adds NTP functionality to the application
 */
class Ntp : public Module {
  private:
    ConfigBool *enable = new ConfigBool("OI/ntp", "enable", false, true);
    ConfigString *server1 = new ConfigString("OI/ntp", "server1", "pool.ntp.org", true);
    ConfigString *server2 = new ConfigString("OI/ntp", "server2", "", true);
    ConfigString *server3 = new ConfigString("OI/ntp", "server3", "", true);
    ConfigLong *offsetGmt = new ConfigLong("OI/ntp", "offsetGmt", 3600, true);
    ConfigLong *offsetDst = new ConfigLong("OI/ntp", "offsetDst", 3600, true);

  public:
    void setup();
    void loop(bool firstLoop) {};
    Ntp();

    //--- Configuration ------------------------------------------------------------
    void setEnable(bool enable_) { enable->setValue(enable_); }
    void setServer1(const char *server1_) { server1->setValue(server1_); }
    void setServer2(const char *server2_) { server2->setValue(server2_); }
    void setServer3(const char *server3_) { server3->setValue(server3_); }
    void setOffsetGmt(long offsetGmt_) { offsetGmt->setValue(offsetGmt_); }
    void setOffsetDst(long offsetDst_) { offsetGmt->setValue(offsetDst_); }

    bool getEnable() { return enable->getValue(); }
    String getServer1() { return server1->getValue(); }
    String getServer2() { return server2->getValue(); }
    String getServer3() { return server3->getValue(); }
    long getOffsetGmt() { return offsetGmt->getValue(); }
    long getOffsetDst() { return offsetDst->getValue(); }

    //--- Utility function to print the local time ----------------------------------
    void printLocalTime(char *buf, int maxlen, const char *format);

    ~Ntp() = default;

  private:
    void logSettings();
};

#define ntp getNtp()

Ntp *getNtp();

#endif