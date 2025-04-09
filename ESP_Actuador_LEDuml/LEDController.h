#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <Arduino.h>

class LEDController {
private:
    int rLedPin;
    int yLedPin;
    int gLedPin;

public:
    LEDController(int redPin, int yellowPin, int greenPin);
    void initialize();
    void setLED(String ledName, String state);
    void processCommand(String command);
    void processMultipleCommands(String input);
};

#endif