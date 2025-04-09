#ifndef SERVER_MANAGER_H
#define SERVER_MANAGER_H

#include <WiFiClient.h>

class ServerManager {
private:
    WiFiClient client;
    const char* serverIp;
    const int serverPort;
    const unsigned long timeoutMs;
    const unsigned long reconnectInterval;
    unsigned long lastReconnectAttempt;

public:
    ServerManager(const char* serverIp, int serverPort, 
                 unsigned long timeoutMs = 5000, 
                 unsigned long reconnectInterval = 5000);
    bool connectToServer();
    bool isConnected();
    void checkConnection();
    void sendMessage(const char* message);
    String readMessage();
};

#endif