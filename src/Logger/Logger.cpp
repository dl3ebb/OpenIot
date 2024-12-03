#include "Logger.h"

/**
 * Convinience methods for the different log levels
 */
void Logger::debug(const __FlashStringHelper *format, ...) {
#ifdef DEBUG
    va_list args;
    va_start(args, format);
    logMessageInt(0, format, args);
#endif
}

void Logger::info(const __FlashStringHelper *format, ...) {
    va_list args;
    va_start(args, format);
    logMessageInt(1, format, args);
}

void Logger::warn(const __FlashStringHelper *format, ...) {
    va_list args;
    va_start(args, format);
    logMessageInt(2, format, args);
}

void Logger::error(const __FlashStringHelper *format, ...) {
    va_list args;
    va_start(args, format);
    logMessageInt(3, format, args);
}

Logger *_logger = nullptr;

Logger *getLogger() {
    if (!_logger) {
        _logger = new Logger();
    }
    return _logger;
}
