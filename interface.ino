#include <Arduino.h>

#include "networking.h"
#include "serialCommands.h"

using namespace Tracker;

// Create instances of the plugins
SerialCommands::SerialCommands serialCommands;
Networking::Networking networking;
Logger::Logger logger("Main");

void setup() {
    // Initialize serial
    Serial.begin(115200);

    // Initialize logger
    logger.info("Starting setup");

    // Initialize modules
    serialCommands.begin();
    // Initialize Networking with settings from serial commands
    Networking::NetworkSettings settings(
        serialCommands.getSSID().c_str(), serialCommands.getPassword().c_str(),
        serialCommands.getHost().c_str(), serialCommands.getPort());
    networking.begin(settings);

    logger.info("Setup complete");
}

void loop() {
    // Handle modules
    serialCommands.handleDelay(10);
    // If network not connected, delay but accept serial commands
    if (!networking.handle()) {
        serialCommands.handleDelay(1000);
        return;
    }

    // Send IMU data and check for connection response
    if (!networking.sendMessage("Hello world!")) {
        // serialCommands.handleDelay(1000);
    }
}