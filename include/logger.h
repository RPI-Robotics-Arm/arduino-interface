#pragma once
#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <Arduino.h>
#include <stdarg.h>
#include <cstring>

namespace Tracker
{
    namespace Logger
    {
        class Logger
        {
        public:
            enum LogLevel
            {
                DEBUG,
                INFO,
                WARN,
                ERROR
            };

            Logger(const char *prefix, LogLevel level = INFO);

            void log(LogLevel level, const char *format, va_list args);
            void debug(const char *format, ...);
            void info(const char *format, ...);
            void warn(const char *format, ...);
            void error(const char *format, ...);

        private:
            void printLog(const char *levelStr, const char *message);
            const char *levelToString(LogLevel level);

            const char *_prefix;
            LogLevel _level;
        };
    }
}

#endif