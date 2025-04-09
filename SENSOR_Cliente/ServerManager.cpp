#include "ServerManager.h"

ServerManager::ServerManager(const char* serverIp, int serverPort, unsigned long timeoutMs)
    : serverIp(serverIp), serverPort(serverPort), timeoutMs(timeoutMs) {}

bool ServerManager::connectToServer() {
    Serial.println("Conectando al servidor...");
    if (client.connect(serverIp, serverPort)) {
        Serial.println("Conectado al servidor");
        client.println("SENSOR");
        return true;
    }
    Serial.println("Error al conectar al servidor");
    return false;
}

void ServerManager::requestIntervals() {
    if (!client.connected()) return;

    client.println("GET_INTERVALS");
    Serial.println("Solicitando intervalos al servidor...");

    unsigned long startTime = millis();
    while (millis() - startTime < timeoutMs) {
        if (client.available()) {
            String response = client.readStringUntil('\n');
            response.trim();
            processIntervalResponse(response);
            break;
        }
        delay(10);
    }
}

void ServerManager::sendIntervalUpdate(int interval) {
    if (!client.connected()) return;
    String command = "PUT " + String(interval);
    client.println(command);
    Serial.println("Enviado: " + command);
}

void ServerManager::processIntervalResponse(const String& response) {
    int startPos = 0;
    while (startPos < response.length()) {
        int endPos = response.indexOf('\n', startPos);
        if (endPos == -1) endPos = response.length();

        String line = response.substring(startPos, endPos);
        line.trim();

        if (line.length() > 0) {
            int comma1 = line.indexOf(',');
            int comma2 = line.indexOf(',', comma1 + 1);

            if (comma1 != -1 && comma2 != -1) {
                int index = line.substring(0, comma1).toInt();
                float minVal = line.substring(comma1 + 1, comma2).toFloat();
                float maxVal = line.substring(comma2 + 1).toFloat();

                if (index >= 0 && index < 4) {
                    // Actualizar intervalos (necesitarías acceso a la estructura de intervalos)
                    Serial.printf("Intervalo %d actualizado: %.1f - %.1f\n", index, minVal, maxVal);
                }
            }
        }
        startPos = endPos + 1;
    }
}