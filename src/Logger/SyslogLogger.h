#ifndef SYSLOG_LOGGER_H
#define SYSLOG_LOGGER_H
#include "Logger.h"
#include <Core/AppHostname.h>
#include <Core/Config/Config.h>
#include <Logger/Logger.h>

#define QUEUE_SIZE 50

#define PRI_ERROR 3
#define PRI_WARNING 4
#define PRI_INFO 6
#define PRI_DEBUG 7

#define FAC_LOCAL7 23

class SyslogLogger : public Logger {
  private:
    //--- Config ---------------------
    ConfigBool *enable = new ConfigBool("OI/syslog", "enable", false, true);
    ConfigString *logServer = new ConfigString("OI/syslog", "server", "", true);
    ConfigLong *serverPort = new ConfigLong("OI/syslog", "serverport", 514, true);

    //--- Queues for the Messages --------
    String messageQueue[QUEUE_SIZE]; // Die Nachrichten
    int messageSeverity[QUEUE_SIZE]; // Severity der Nachrichten

    volatile int queueFront = 0;
    volatile int queueBack = 0;
    volatile int queueCount = 0;
    portMUX_TYPE queueMux = portMUX_INITIALIZER_UNLOCKED;

    void print(uint8_t facility, uint8_t severity, char *mesg);

    Logger *backupLogger = new Logger();

  public:
    SyslogLogger();
    void logMessageInt(int level, const __FlashStringHelper *format, va_list args) override;
    void _sendMessages();

    //----Configuration ---------------------------------------
    void setLogServer(const char *logServer_) { logServer->setValue(logServer_); }
    void setServerPort(long serverPort_) { serverPort->setValue(serverPort_); }
    void setEnable(bool enable_) { enable->setValue(enable_); }
    void setBackupLogger(Logger *backupLogger_) { backupLogger = backupLogger_; }

    String getLogServer() { return logServer->getValue(); }
    long getServerPort() { return serverPort->getValue(); }
    bool getEnable() { return enable->getValue(); }
};

#define syslogLogger getSyslogLogger()

SyslogLogger *getSyslogLogger();

#endif