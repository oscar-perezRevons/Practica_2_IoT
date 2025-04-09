#include "ServerManager.h"

ServerManager::ServerManager(const char* serverIp, int serverPort, 
                           unsigned long timeoutMs, 
                           unsigned long reconnectInterval)
    : serverIp(serverIp), serverPort(serverPort), 
      timeoutMs(timeoutMs), reconnectInterval(reconnectInterval),
      lastReconnectAttempt(0) {}

bool ServerManager::connectToServer() {
    Serial.println("Conectando al servidor...");
    if (client.connect(serverIp, serverPort)) {
        Serial.println("Conectado al servidor");
        client.println("LED_CONTROLLER");
        return true;
    }
    Serial.println("Error al conectar al servidor");
    return false;
}

bool ServerManager::isConnected() {
    return client.connected();
}

void ServerManager::checkConnection() {
    if (!client.connected()) {
        unsigned long currentTime = millis();
        if (currentTime - lastReconnectAttempt > reconnectInterval) {
            lastReconnectAttempt = currentTime;
            connectToServer();
        }
    }
}

void ServerManager::sendMessage(const char* message) {
    if (client.connected()) {
        client.println(message);
    }
}

String ServerManager::readMessage() {
    if (client.available()) {
        return client.readStringUntil('\n');
    }
    return "";
}