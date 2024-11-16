#pragma once
#ifndef _SERIAL_COMMANDS_H_
#define _SERIAL_COMMANDS_H_

#include <Arduino.h>
#include <SPIFFS.h>
#include "Logger.h" // Include the logger header

namespace Tracker
{
    namespace SerialCommands
    {
        class SerialCommands
        {
        public:
            SerialCommands();
            void begin();
            void handle();                                   // Updated function name
            void handleDelay(unsigned long delayTimeMs = 0); // New function for hanging and handling serial comamands

            // Accessors
            String getSSID() const;
            String getPassword() const;
            String getHost() const;
            int getPort() const;

        private:
            // Update network settings with provided SSID and password
            void updateNetworkSettings(const String &ssid, const String &password);
            // Update server settings with provided host and port
            void updateServerSettings(const String &host, int port);
            // Print available commands to the serial monitor
            void printHelp();
            // Print network settings
            void printNetworkSettings();
            // Save current settings to the SPIFFS file
            bool saveSettings();
            // Load settings from the SPIFFS file
            bool loadSettings();
            // Reset the device
            void resetDevice();

            String _ssid;     // Network SSID
            String _password; // Network password
            String _host;     // Server host address
            int _port;        // Server port

            static const char *SETTINGS_FILE; // Path to settings file in SPIFFS
            Tracker::Logger::Logger logger;   // Logger instance for logging
        };
    }
}

#endif
