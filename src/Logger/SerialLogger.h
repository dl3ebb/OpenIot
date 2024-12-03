#ifndef SERIAL_LOGGER_H
#define SERIAL_LOGGER_H
#include "Logger.h"

class SerialLogger : public Logger {

  public:
    SerialLogger();
    void setBaudrate(unsigned long baudRate);
    void logMessageInt(int level, const __FlashStringHelper *format, va_list args) override;
};

#define serialLogger getSerialLogger()

SerialLogger *getSerialLogger();

#endif