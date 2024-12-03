#ifndef LOGGER_H
#define LOGGER_H
#include "Arduino.h"
/**
 * The logger class defines an abstract base class for all Logegrs.
 *
 * Basically there are two differnt loggers : The serial logger, that logs to the console
 * and the syslogLogger that logs to a remote logging host.
 *
 * For the syslogger there may be a backup logger defined in case the syslogger is not available. Usually this will be the serial logger.
 *
 * Debug messages are only sent out while in the development environment and filtered out in the production image
 */
class Logger {
  public:
    void debug(const __FlashStringHelper *format, ...);
    void info(const __FlashStringHelper *format, ...);
    void warn(const __FlashStringHelper *format, ...);
    void error(const __FlashStringHelper *format, ...);
    virtual void logMessageInt(int level, const __FlashStringHelper *format, va_list args) {};
};

#define logger getLogger()

Logger *getLogger();

#endif