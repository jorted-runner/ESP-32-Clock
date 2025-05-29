#include <Arduino.h>
#include "WiFiManager.h"
#include "TimeAPICall.h"
#include "OutputOnDisplay.h"
#include <U8g2lib.h>

void wifiAndTimeTask(void * parameter);

void setup() {
  //this establishes a serial connection with the PC. This makes debugging a lot easier
  Serial.begin(115200);

  //initialize the I2C connection with the display
  u8g2.begin();

  //the delay here is to make sure the connection is established and no communication starts without the PC being ready for it
  delay(1000);
  
  // Connect to WiFi using the WiFiManager module  
  connectToWiFi();

  //this gets the time from the API
  //utcOffset is set to 0 at the start, it is there to set your timezone
  //dstOffset is also set to 0, it defines whether or not there is daylight savings time observed
  configTime(utcOffset, dstOffset, NTP_SERVER);

  // Create task for keeping clock in sync and manage wifi connection
  // pinning it to core 0 to keep it run it in parallel to the main loop, which is run in core 1
  xTaskCreatePinnedToCore(
    wifiAndTimeTask,  // Task Function
    "WiFiTimeTask",   // Task Name
    4096,             // Stack size (in words, not bytes)
    NULL,             // Task input
    1,                // Priority Level
    NULL,             // Task handle, which we dont need
    0                 // Setting the task to run on Core 0
  );
}

void loop() {
  prepareDisplay();
  sendTimeToDisplayBuffer(); 
  outputOnDisplay();
}

void wifiAndTimeTask(void * parameter) { // FreeRTOS allows passing parameter to the task when created. Even though we aren't passing any parameters, this must be included because FreeRTOS expects the signature
  for (;;) { // forever loop, its the same as using while(true)
    bool connected = checkWiFiAndReconnect();
    Serial.println("\nWiFi checked");

    // if the wifi is connected, call the api to update the time
    if (connected) {
      configTime(utcOffset, dstOffset, NTP_SERVER);
      Serial.println("\nTime synced");
    }

    vTaskDelay(10000 / portTICK_PERIOD_MS); // I have this set to wait 10 seconds for testing, but we can change it to update at any interval we want
  }
}
