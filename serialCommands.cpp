#include "serialCommands.h"

namespace Tracker
{
    namespace SerialCommands
    {
        // Path to the settings file in SPIFFS
        const char *SerialCommands::SETTINGS_FILE = "/settings.txt";

        SerialCommands::SerialCommands() : logger("SerialCommands")
        {
            // Initialize member variables
            _ssid = "";
            _password = "";
            _host = "";
            _port = 0;
        }

        void SerialCommands::begin()
        {
            if (!SPIFFS.begin(true))
            {
                logger.error("Failed to mount file system");
                handleDelay(0); // Await serial commands indefinitely
                return;
            }

            if (!loadSettings())
            {
                logger.error("Settings file does not exist");
                handleDelay(0); // Await serial commands indefinitely
                return;
            }

            logger.info("Settings loaded successfully");
        }

        void SerialCommands::handle()
        {
            if (Serial.available() <= 0)
            {
                return; // No data available, exit early
            }

            // Read and process the command from serial input
            String command = Serial.readStringUntil('\n');
            command.trim();

            if (command.startsWith("update ssid "))
            {
                String ssid = command.substring(12);
                updateNetworkSettings(ssid, _password);
                return;
            }

            if (command.startsWith("update password "))
            {
                String password = command.substring(16);
                updateNetworkSettings(_ssid, password);
                return;
            }

            if (command.startsWith("update server "))
            {
                int spaceIndex = command.indexOf(' ', 14);
                if (spaceIndex != -1)
                {
                    String host = command.substring(14, spaceIndex);
                    int port = command.substring(spaceIndex + 1).toInt();
                    updateServerSettings(host, port);
                }
                return;
            }

            if (command.equals("help"))
            {
                printHelp();
                return;
            }

            if (command.equals("reset"))
            {
                resetDevice();
                return;
            }

            if (command.equals("settings"))
            {
                printNetworkSettings();
                return;
            }

            logger.error("Invalid command: %s", command.c_str());
        }

        void SerialCommands::handleDelay(unsigned long delayTimeMs)
        {
            const unsigned long interval = 10; // Update interval in milliseconds

            // Check if delayTimeMs is 0 to loop forever
            if (delayTimeMs == 0)
            {
                while (true)
                {
                    handle();        // Handle serial commands
                    delay(interval); // Small delay to prevent tight loop
                }
            }

            // Record the start time
            unsigned long startMillis = millis();

            while (true)
            {
                // Exit the loop if the specified delay time has elapsed
                if (millis() - startMillis >= delayTimeMs)
                {
                    break;
                }

                handle();
                delay(interval);
            }
        }

        void SerialCommands::updateNetworkSettings(const String &ssid, const String &password)
        {
            _ssid = ssid;
            _password = password;
            if (!saveSettings())
            {
                logger.error("Failed to update network settings");
                return;
            }
            logger.info("Network settings updated: SSID=%s, Password=%s", _ssid.c_str(), _password.c_str());
        }

        void SerialCommands::updateServerSettings(const String &host, int port)
        {
            _host = host;
            _port = port;
            if (!saveSettings())
            {
                logger.error("Failed to update server settings");
                return;
            }
            logger.info("Server settings updated: Host=%s, Port=%d", _host.c_str(), _port);
        }

        void SerialCommands::printHelp()
        {
            Serial.println("Available commands:");
            Serial.println("  update ssid <ssid>          - Update the network SSID");
            Serial.println("  update password <password>  - Update the network password");
            Serial.println("  update server <host> <port> - Update the server host and port");
            Serial.println("  help                        - Display this help message");
            Serial.println("  reset                       - Reset the device");
            Serial.println("  settings                    - Display stored network settings");
            logger.info("Displayed help information");
        }

        void SerialCommands::printNetworkSettings()
        {
            Serial.println("Stored Network Settings:");
            Serial.printf("  SSID:           %s\n", _ssid.c_str());
            Serial.printf("  Password:       %s\n", _password.c_str());
            Serial.printf("  Server Host:    %s\n", _host.c_str());
            Serial.printf("  Server Port:    %d\n", _port);
            logger.info("Displayed network settings");
        }

        bool SerialCommands::saveSettings()
        {
            File file = SPIFFS.open(SETTINGS_FILE, FILE_WRITE);
            if (!file)
            {
                logger.error("Failed to open settings file for writing");
                return false;
            }

            file.println(_ssid);
            file.println(_password);
            file.println(_host);
            file.println(_port);

            file.close();
            logger.info("Settings saved to file");
            return true;
        }

        bool SerialCommands::loadSettings()
        {
            File file = SPIFFS.open(SETTINGS_FILE, FILE_READ);
            if (!file)
            {
                logger.error("Settings file does not exist");
                return false;
            }

            _ssid = file.readStringUntil('\n');
            _ssid.trim();
            _password = file.readStringUntil('\n');
            _password.trim();
            _host = file.readStringUntil('\n');
            _host.trim();
            _port = file.readStringUntil('\n').toInt();

            file.close();
            logger.info("Settings loaded from file");
            return true;
        }

        void SerialCommands::resetDevice()
        {
            logger.info("Resetting device...");
            // Perform a soft reset
            ESP.restart();
        }

        // Accessor methods
        String SerialCommands::getSSID() const
        {
            return _ssid;
        }

        String SerialCommands::getPassword() const
        {
            return _password;
        }

        String SerialCommands::getHost() const
        {
            return _host;
        }

        int SerialCommands::getPort() const
        {
            return _port;
        }
    }
}
