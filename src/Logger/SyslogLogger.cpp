#include "SyslogLogger.h"
#include <Modules/Wifi.h>
#include <OpenIot.h>
#include <Ticker.h>
#include <WiFi.h>

Ticker messageTicker;
WiFiUDP logServerUdp;

void sendLogs() {
    syslogLogger->_sendMessages();
}

SyslogLogger::SyslogLogger() : Logger() {
    messageTicker.attach(0.03, ::sendLogs);
}

void SyslogLogger::logMessageInt(int level, const __FlashStringHelper *format, va_list args) {

    if (enable && logServer->getValue() != "" && wifi->getNetworkStatus() == NetworkStatus::STATION) {
        char message[256];
        vsnprintf(message, 256, (const char *)format, args);
        va_end(args);

        switch (level) {
        case 0:
            print(FAC_LOCAL7, PRI_DEBUG, message);
            break;
        case 1:
            print(FAC_LOCAL7, PRI_INFO, message);
            break;
        case 2:
            print(FAC_LOCAL7, PRI_WARNING, message);
            break;
        case 3:
            print(FAC_LOCAL7, PRI_ERROR, message);
            break;
        }
    } else {
        backupLogger->logMessageInt(level, format, args);
    }
}

void SyslogLogger::print(uint8_t facility, uint8_t severity, char *mesg) {
    // The PRI value is an integer number which calculates by the following metric:
    uint8_t priority = (8 * facility) + severity;
    String syslogMsg = String(mesg);

    //--- Put it into the queue ------------------------------------
    portENTER_CRITICAL(&queueMux);
    if (queueCount < QUEUE_SIZE) {
        messageQueue[queueBack] = syslogMsg;
        messageSeverity[queueBack] = priority;
        queueBack = (queueBack + 1) % QUEUE_SIZE;
        queueCount++;
    }
    portEXIT_CRITICAL(&queueMux);
}

void SyslogLogger::_sendMessages() {
    String message;
    int mSeverity = -1;
    portENTER_CRITICAL(&queueMux);
    if (queueCount > 0) {
        message = messageQueue[queueFront];
        mSeverity = messageSeverity[queueFront];
        queueFront = (queueFront + 1) % QUEUE_SIZE;
        queueCount--;
    }
    portEXIT_CRITICAL(&queueMux);

    if (mSeverity != -1) {

        String syslogMsg = "<" + String(mSeverity) + ">" + appHostname->getHostname() + " " + openIot.getApplicationName() + "_b" + String(openIot.getApplicationBuild()) + ": " + message;
        logServerUdp.beginPacket(logServer->getValue().c_str(), static_cast<uint16_t>(serverPort->getValue()));
        logServerUdp.print(syslogMsg);
        logServerUdp.endPacket();
    }
}

SyslogLogger *_syslogLogger = nullptr;

SyslogLogger *getSyslogLogger() {
    if (!_syslogLogger) {
        _syslogLogger = new SyslogLogger();
    }
    return _syslogLogger;
}
