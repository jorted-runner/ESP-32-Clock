#include <Arduino.h>
#include "WiFiManager.h"
#include "TimeAPICall.h"



void setup() {
  //this establishes a serial connection with the PC. This makes debugging a lot easier
  Serial.begin(115200);
  //the delay here is to make sure the connection is established and no communication starts without the PC being ready for it
  delay(1000);
  
  // Connect to WiFi using the WiFiManager module  
  connectToWiFi();

  //this gets the time from the API
  //utcOffset is set to 0 at the start, it is there to set your timezone
  //dstOffset is also set to 0, it defines whether or not there is daylight savings time observed
  configTime(utcOffset, dstOffset, NTP_SERVER);
}

void loop() {
  printLocalTime();
  delay(1000);
}
