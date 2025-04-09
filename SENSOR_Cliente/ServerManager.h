#ifndef SERVER_MANAGER_H
#define SERVER_MANAGER_H

#include <WiFiClient.h>

class ServerManager {
private:
    WiFiClient client;
    const char* serverIp;
    const int serverPort;
    const unsigned long timeoutMs;

public:
    ServerManager(const char* serverIp, int serverPort, unsigned long timeoutMs = 5000);
    bool connectToServer();
    void requestIntervals();
    void sendIntervalUpdate(int interval);

private:
    void processIntervalResponse(const String& response);
};

#endif