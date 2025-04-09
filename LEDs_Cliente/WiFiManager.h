#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

class WiFiManager {
private:
    const char* ssid;
    const char* password;
    const int maxAttempts;

public:
    WiFiManager(const char* ssid, const char* password, int maxAttempts = 10);
    
    void connectToWiFi();
    bool isConnected() const;
};

#endif