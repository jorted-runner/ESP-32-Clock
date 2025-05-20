#include <WiFi.h>
#include <Arduino.h>
#include "Secrets.h"

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Flash the LED to indicate the system setup is running (Did this to stop myself from going crazy)
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);

  // Initialize wifi connection
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  Serial.println("\nConnecting");
  digitalWrite(LED_BUILTIN, HIGH);

  // This loops until the wifi is connected
  while(WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print(".");
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);

  }

  // Prints off the IP address
  Serial.println("\nConnected to the WiFi netword");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());

  // Turns off the LED after setup is complete
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  Serial.println("Still on...");
  delay(1000);
}