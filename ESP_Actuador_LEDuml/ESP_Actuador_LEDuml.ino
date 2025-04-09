#include <Arduino.h>
#include "WiFiManager.h"
#include "ServerManager.h"
#include "LEDController.h"

// Configuración
WiFiManager wifiManager("TECHLAB", "catolica11");
ServerManager serverManager("192.168.75.46", 12345);
LEDController ledController(25, 26, 27); // Pines para los LEDs

void setup() {
    Serial.begin(115200);
    ledController.initialize();
    wifiManager.connectToWiFi();
    serverManager.connectToServer();
}

void loop() {
    // Mantener conexión WiFi
    if (!wifiManager.isConnected()) {
        wifiManager.connectToWiFi();
        delay(1000);
        return;
    }

    // Manejar reconexión al servidor
    serverManager.checkConnection();

    // Procesar mensajes del servidor
    String message = serverManager.readMessage();
    if (message.length() > 0) {
        ledController.processMultipleCommands(message);
        serverManager.sendMessage("LEDs actualizados");
    }
    
    delay(10);
}