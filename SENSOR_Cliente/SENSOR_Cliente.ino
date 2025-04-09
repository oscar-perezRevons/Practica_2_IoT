#include <Arduino.h>
#include "WiFiManager.h"
#include "ServerManager.h"
#include "UltrasonicSensor.h"
#include "IntervalManager.h"

// Configuración
WiFiManager wifiManager("TECHLAB", "catolica11");
ServerManager serverManager("192.168.75.46", 12345);
UltrasonicSensor sensor(16, 17);
IntervalManager intervalManager;

void setup() {
    Serial.begin(115200);
    wifiManager.connectToWiFi();

    if (wifiManager.isConnected() && serverManager.connectToServer()) {
        serverManager.requestIntervals();
    }
}

void loop() {
    if (!wifiManager.isConnected()) {
        wifiManager.connectToWiFi();
        delay(1000);
        return;
    }

    if (!serverManager.connectToServer()) {
        delay(5000);
        return;
    }

    float distance = sensor.readDistanceCM();
    int currentInterval = intervalManager.getIntervalIndex(distance);

    if (currentInterval != intervalManager.getLastInterval()) {
        serverManager.sendIntervalUpdate(currentInterval);
        intervalManager.setLastInterval(currentInterval);
    }

    delay(250); // Reducir carga del sistema
}