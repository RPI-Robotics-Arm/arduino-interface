#include "networking.h"
#include <cstdarg>
#include <cstring>

namespace Tracker
{
    namespace Networking
    {
        Networking::Networking() : logger("Networking") {}

        void Networking::begin(const NetworkSettings &settings)
        {
            _settings = settings; // Store the network settings

            logger.info("Initializing networking with SSID: %s, HOST: %s, PORT: %d",
                        _settings.ssid.c_str(), _settings.host.c_str(), _settings.port);

            WiFi.begin(_settings.ssid.c_str(), _settings.password.c_str());
            udp.begin(0); // Start UDP on any available port
            udp.setTimeout(1000);

            logger.info("Networking initialized");
        }

        bool Networking::handle()
        {
            // Check WiFi connection and reconnect if necessary
            if (WiFi.status() != WL_CONNECTED)
            {
                networkStatus = false;
                logger.error("WiFi not connected. Attempting to reconnect...");
                return false;
            }
            else
            {
                if (!networkStatus)
                {
                    networkStatus = true;
                    logger.info("WiFi connected on IP address: %s", WiFi.localIP().toString().c_str());
                }
                return true;
            }
        }

        bool Networking::sendMessage(const char *format, ...)
        {
            va_list args;
            va_start(args, format);
            vsnprintf(buffer, sizeof(buffer), format, args);
            va_end(args);

            // Send the message via UDP
            udp.beginPacket(_settings.host.c_str(), _settings.port);
            udp.write(reinterpret_cast<uint8_t *>(buffer), strlen(buffer));
            if (!udp.endPacket())
            {
                logger.error("Failed to send message");
                return false;
            }

            // Wait for a response with a timeout
            int packetSize = udp.parsePacket();
            if (packetSize <= 0)
            {
                logger.warn("No response received within timeout period");
                return false;
            }

            int len = udp.read(buffer, sizeof(buffer) - 1); // Leave space for null terminator
            if (len <= 0)
            {
                logger.error("Failed to read response");
                return false;
            }

            buffer[len] = '\0'; // Null-terminate the string
            logger.info("Message sent with response: %s", buffer);
            return true;
        }

        bool Networking::connectToServer()
        {
            logger.info("Connecting to server: %s:%d", _settings.host.c_str(), _settings.port);

            if (udp.beginPacket(_settings.host.c_str(), _settings.port))
            {
                logger.info("Connected to server");
                return true;
            }
            else
            {
                logger.error("Connection to server failed");
                return false;
            }
        }
    }
}
