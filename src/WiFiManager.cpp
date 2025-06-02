#include <WiFi.h>
#include "WiFiManager.h"
#include <Arduino.h>

void connectToWiFi(const char* ssid, const char* password) {
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.begin(ssid, password);

    Serial.println("\nConnecting to WiFi...");
    unsigned long startAttemptTime = millis(); // Store current time in milliseconds
    const unsigned long connectionTimeout = 15000; // 15-second timeout

    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < connectionTimeout) {
        Serial.print(".");
        delay(500);
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected to WiFi");
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nFailed to connect to WiFi");
    }
}

bool checkWiFiAndReconnect() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi disconnected. Reconnecting...");
        WiFi.reconnect();

        unsigned long start = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
            Serial.print(".");
            delay(500);
        }

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("\nReconnected!");
            return true;
        } else {
            Serial.println("Reconnection failed");
            return false;
        }
    }

    return true; // Wifi is already connected
}