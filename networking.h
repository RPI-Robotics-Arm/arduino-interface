#pragma once
#ifndef _NETWORKING_H_
#define _NETWORKING_H_

#include <WiFi.h>
#include "logger.h"

namespace Tracker
{
    namespace Networking
    {
        struct NetworkSettings
        {
            String ssid;     // Network SSID
            String password; // Network password
            String host;     // Server host address
            uint16_t port;   // Server port

            // Constructor to initialize members
            NetworkSettings(const String &ssid = "", const String &password = "", const String &host = "", uint16_t port = 0)
                : ssid(ssid), password(password), host(host), port(port) {}
        };

        class Networking
        {
        public:
            Networking();
            void begin(const NetworkSettings &settings);
            bool handle();
            bool sendMessage(const char *format, ...);

        private:
            bool connectToServer();

            NetworkSettings _settings;      // Store network settings
            bool networkStatus = false;     // Is network connected
            char buffer[256];               // Buffer for sending/receiving data
            WiFiUDP udp;                    // UDP instance
            Tracker::Logger::Logger logger; // Logger instance
        };
    }
}

#endif
