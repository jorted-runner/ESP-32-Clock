#include <WiFi.h>
#include "Secrets.h"
#include "WiFiManager.h"
#include <Arduino.h>

void connectToWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    Serial.println("\nConnecting");
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    while(WiFi.status() != WL_CONNECTED) {
        digitalWrite(LED_BUILTIN, LOW);
        Serial.print(".");
        delay(100);
        digitalWrite(LED_BUILTIN, HIGH);
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
}