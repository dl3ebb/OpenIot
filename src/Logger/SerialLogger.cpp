#include "SerialLogger.h"

SerialLogger::SerialLogger() : Logger() {
    Serial.begin(9600);
}

void SerialLogger::setBaudrate(unsigned long baudRate) {
    Serial.updateBaudRate(baudRate);
}

void SerialLogger::logMessageInt(int level, const __FlashStringHelper *format, va_list args) {

    char message[256];
    vsnprintf(message, 256, (const char *)format, args);
    va_end(args);

    switch (level) {
    case 0:
        Serial.print("DEBUG - ");
        break;
    case 1:
        Serial.print("INFO  - ");
        break;
    case 2:
        Serial.print("WARN  - ");
        break;
    case 3:
        Serial.print("ERR   - ");
        break;
    }
    Serial.println(message);
}

SerialLogger *_serialLogger = nullptr;

SerialLogger *getSerialLogger() {
    if (!_serialLogger) {
        _serialLogger = new SerialLogger();
    }
    return _serialLogger;
}
