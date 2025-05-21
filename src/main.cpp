#include <Arduino.h>
#include "WiFiManager.h"

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
  
  // Connect to WiFi using the WiFiManager module  
  connectToWiFi();

  //this gets the time from the API
  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);
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