#include "logger.h"

namespace Tracker
{
    namespace Logger
    {
        Logger::Logger(const char *prefix, LogLevel level)
            : _prefix(prefix), _level(level) {}

        void Logger::log(LogLevel level, const char *format, va_list args)
        {
            if (level < _level)
            {
                return;
            }

            char buffer[256];
            vsnprintf(buffer, sizeof(buffer), format, args);

            const char *levelStr = levelToString(level);
            printLog(levelStr, buffer);
        }

        void Logger::debug(const char *format, ...)
        {
            va_list args;
            va_start(args, format);
            log(DEBUG, format, args);
            va_end(args);
        }

        void Logger::info(const char *format, ...)
        {
            va_list args;
            va_start(args, format);
            log(INFO, format, args);
            va_end(args);
        }

        void Logger::warn(const char *format, ...)
        {
            va_list args;
            va_start(args, format);
            log(WARN, format, args);
            va_end(args);
        }

        void Logger::error(const char *format, ...)
        {
            va_list args;
            va_start(args, format);
            log(ERROR, format, args);
            va_end(args);
        }

        void Logger::printLog(const char *levelStr, const char *message)
        {
            Serial.printf("[%s] [%s] %s\n", _prefix, levelStr, message);
        }

        const char *Logger::levelToString(LogLevel level)
        {
            switch (level)
            {
            case DEBUG:
                return "DEBUG";
            case INFO:
                return "INFO";
            case WARN:
                return "WARN";
            case ERROR:
                return "ERROR";
            default:
                return "INFO";
            }
        }
    } // namespace Logger
} // namespace Tracker
