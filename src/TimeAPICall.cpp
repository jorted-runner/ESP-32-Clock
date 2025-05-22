#include <Arduino.h>

//this function is just to check if the api call works correctly. In a later stage this would be replaced
//with an output to the display!
void printLocalTime() {
  //this will get the current time
  struct tm timeinfo;

  //this will check if the time is returned in a correct format
  if (!getLocalTime(&timeinfo)) {
    //lets us know if the api was called correctly.
    Serial.println("Connection Err");

    //this line is simply for debugging purposes
    Serial.println(&timeinfo);

    return;
  }

  //this will print out the time correctly
  Serial.println(&timeinfo, "%H:%M:%S");
  Serial.println(&timeinfo, "%d/%m/%Y   %Z");
}