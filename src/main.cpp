#include <WiFi.h>
#include <Arduino.h>
#include "Secrets.h"

//these are all the time related constants
//this is the link to the public api
#define NTP_SERVER     "time.google.com"
//this will define the timezone
#define UTC_OFFSET     0
//this will define whether or not there is daylight savings
#define UTC_OFFSET_DST 0

//here are all the functions declarations
void printLocalTime();

void setup() {
  //this establishes a serial connection with the PC. This makes debugging a lot easier
  Serial.begin(115200);
  //the delay here is to make sure the connection is established and no communication starts without the PC being ready for it
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

  //this gets the time from the API
  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);

  // Turns off the LED after setup is complete
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  printLocalTime();
  delay(1000);
}

//this function is just to check if the api call works correctly. In a later stage this would be replaced
//with an output to the display!
void printLocalTime() {
  //this will get the current time
  struct tm timeinfo;

  //this will check if the time is returned in a correct format
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Connection Err");

    //this line is simply for debugging purposes
    Serial.println(&timeinfo);

    return;
  }

  //this will print out the time correctly
  Serial.println(&timeinfo, "%H:%M:%S");
  Serial.println(&timeinfo, "%d/%m/%Y   %Z");
}