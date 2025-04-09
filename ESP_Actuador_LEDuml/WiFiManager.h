#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

class WiFiManager {
private:
    const char* ssid;
    const char* password;
    const int maxAttempts;

public:
    // Constructor
    WiFiManager(const char* ssid, const char* password, int maxAttempts = 10);
    
    // Métodos públicos
    void connectToWiFi();
    bool isConnected() const;
};

#endif