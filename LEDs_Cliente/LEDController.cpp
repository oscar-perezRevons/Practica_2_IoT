#include "LEDController.h"

LEDController::LEDController(int redPin, int yellowPin, int greenPin) 
    : rLedPin(redPin), yLedPin(yellowPin), gLedPin(greenPin) {}

void LEDController::initialize() {
    pinMode(rLedPin, OUTPUT);
    pinMode(yLedPin, OUTPUT);
    pinMode(gLedPin, OUTPUT);
    
    digitalWrite(rLedPin, LOW);
    digitalWrite(yLedPin, LOW);
    digitalWrite(gLedPin, LOW);
}

void LEDController::setLED(String ledName, String state) {
    int pin;
    if (ledName == "rLed") pin = rLedPin;
    else if (ledName == "yLed") pin = yLedPin;
    else if (ledName == "gLed") pin = gLedPin;
    else return;
    
    digitalWrite(pin, state == "ON" ? HIGH : LOW);
    Serial.println(ledName + " " + state);
}

void LEDController::processCommand(String command) {
    if (!command.startsWith("POST")) return;
    
    Serial.println("Comando recibido: " + command);
    
    String parts[3];
    int partIndex = 0;
    
    int start = 5;  // Saltar "POST "
    int end = command.indexOf(' ', start);
    
    while (end != -1 && partIndex < 3) {
        parts[partIndex++] = command.substring(start, end);
        start = end + 1;
        end = command.indexOf(' ', start);
    }
    if (partIndex < 3) {
        parts[partIndex] = command.substring(start);
    }
    
    for (int i = 0; i < 3; i++) {
        int colonPos = parts[i].indexOf(':');
        if (colonPos != -1) {
            String led = parts[i].substring(0, colonPos);
            String state = parts[i].substring(colonPos + 1);
            setLED(led, state);
        }
    }
}

void LEDController::processMultipleCommands(String input) {
    int startIndex = 0;
    int postIndex = input.indexOf("POST", startIndex);
    
    while (postIndex != -1) {
        int nextPost = input.indexOf("POST", postIndex + 4);
        String singleCommand;
        
        if (nextPost != -1) {
            singleCommand = input.substring(postIndex, nextPost);
        } else {
            singleCommand = input.substring(postIndex);
        }
        
        singleCommand.trim();
        processCommand(singleCommand);
        postIndex = nextPost;
    }
}