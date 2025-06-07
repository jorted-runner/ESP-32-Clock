#include <Arduino.h>
#include "OutputOnDisplay.h"
#include "PreferencesGlobals.h"

//Depending on your screen, you need to uncomment either one of them. Just try out, which one works, 
//in case you arent sure which one you need
//In case you know which driver you are using (sh1106 or ssd1306) you just need to uncomment that one and comment
//out the other one.

//U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, SCL_PIN, SDA_PIN, U8X8_PIN_NONE);

//this function sets all the required flags for the display, and clears whatever was on there before
void prepareDisplay(){
    u8g2.clearBuffer();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
}

//will get time from rtc and send it to the buffer, ready to be printed out on the display, once outputOnDisplay function is called
void sendTimeToDisplayBuffer(){
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return;
    }
    clockPrefs.begin("clockPrefs", false);
    

    int hoursInCorrectFormat;

    if (clockPrefs.getBool("24hour", true)){
        hoursInCorrectFormat = timeinfo.tm_hour;
    } else {
        hoursInCorrectFormat = timeinfo.tm_hour % 12;
        if (hoursInCorrectFormat == 0) {
            hoursInCorrectFormat = 12;
        }
    }

    clockPrefs.end();

    //this part is used to take the timeinfo, and convert the hours and minutes to an array of chars
    //this is done, since the u8g2.drawStr method is expecting it to be of that type.
    char timeStr[6]; //HH:MM + null terminator

    if (timeinfo.tm_sec % 2 == 1){
        snprintf(timeStr, sizeof(timeStr), "%02d:%02d", hoursInCorrectFormat, timeinfo.tm_min);
    } else {
        snprintf(timeStr, sizeof(timeStr), "%02d %02d", hoursInCorrectFormat, timeinfo.tm_min);
    }
    
    
    //here we set the font
    u8g2.setFont(u8g2_font_profont22_tr);

    //this is where we actually write the time to the buffer
    u8g2.drawStr(59, 20, timeStr);

    clockPrefs.begin("clockPrefs", false);
    if (clockPrefs.getBool("24hour", true)){

    } else {
        if (timeinfo.tm_hour <=12){
        u8g2.drawFrame(59, 0, 58, 5);
        } else {
            u8g2.drawBox(59, 0, 58, 5);
        }
    }
    clockPrefs.end();

}

//will get date from rtc and send it to the buffer, ready to be printed out on the display, once outputOnDisplay function is called
void sendDateToDisplayBuffer(){
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return;
    }

    //this part is used to take the timeinfo, and convert the days and months to an array of chars
    //this is done, since the u8g2.drawStr method is expecting it to be of that type.
    char dateStr[6]; //DD/MM + null terminator
    snprintf(dateStr, sizeof(dateStr), "%02d/%02d", timeinfo.tm_mday, timeinfo.tm_mon + 1); //the month needs +1 since its 0 based


    //here we set the font
    u8g2.setFont(u8g2_font_profont22_tr);

    //this is where we actually write the date to the buffer
    u8g2.drawStr(59, 44, dateStr);
}

void sendAnalogTimeToDisplayBuffer(){
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return;
    }

    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);

    u8g2.drawEllipse(CENTER_ANALOG_CLOCK_X, CENTER_ANALOG_CLOCK_Y, RADIUS_ANALOG_CLOCK, RADIUS_ANALOG_CLOCK);

    //these are small indicators at 12, 3, 6 and 9 o'clock. This just makes it easer to read it.
    u8g2.drawLine(CENTER_ANALOG_CLOCK_X, CENTER_ANALOG_CLOCK_Y - RADIUS_ANALOG_CLOCK + 1, CENTER_ANALOG_CLOCK_X, CENTER_ANALOG_CLOCK_Y - RADIUS_ANALOG_CLOCK + 3);
    u8g2.drawLine(CENTER_ANALOG_CLOCK_X, CENTER_ANALOG_CLOCK_Y + RADIUS_ANALOG_CLOCK - 1, CENTER_ANALOG_CLOCK_X, CENTER_ANALOG_CLOCK_Y + RADIUS_ANALOG_CLOCK - 3);
    u8g2.drawLine(CENTER_ANALOG_CLOCK_X - RADIUS_ANALOG_CLOCK + 1, CENTER_ANALOG_CLOCK_Y, CENTER_ANALOG_CLOCK_X - RADIUS_ANALOG_CLOCK + 3, CENTER_ANALOG_CLOCK_Y);
    u8g2.drawLine(CENTER_ANALOG_CLOCK_X + RADIUS_ANALOG_CLOCK - 1, CENTER_ANALOG_CLOCK_Y, CENTER_ANALOG_CLOCK_X + RADIUS_ANALOG_CLOCK - 3, CENTER_ANALOG_CLOCK_Y);


    //HOUR HAND
    //first we need to get the minutes in seconds, this is done so the minute hand isnt only moving once a minute
    int totalHoursSeconds = ((timeinfo.tm_hour * 60 * 60) + ((timeinfo.tm_min * 60) + timeinfo.tm_sec));
    //next we need to calculate the degrees from these seconds
    float angleHoursDeg = (totalHoursSeconds / (12 * 3600.0)) * 360.0;
    //we need to convert these degrees into a radiant so it works with the sin() and cos() functions
    float angleHoursRad = radians(angleHoursDeg);
    //next we actually draw the hand
    drawClockHands(angleHoursRad, LENGTH_HOUR_HAND, CENTER_ANALOG_CLOCK_X, CENTER_ANALOG_CLOCK_Y);


    //MINUTE HAND
    //first we need to get the minutes in seconds, this is done so the minute hand isnt only moving once a minute
    int totalMinutesSeconds = (timeinfo.tm_min * 60) + timeinfo.tm_sec;
    //next we need to calculate the degrees from these seconds
    float angleMinutesDeg = (totalMinutesSeconds / 3600.0) * 360.0;
    //we need to convert these degrees into a radiant so it works with the sin() and cos() functions
    float angleMinutesRad = radians(angleMinutesDeg);
    //next we actually draw the hand
    drawClockHands(angleMinutesRad, LENGTH_MINUTE_HAND, CENTER_ANALOG_CLOCK_X, CENTER_ANALOG_CLOCK_Y);
    
}

void prepareClockDisplayArea() {
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);

    // Clear only the top portion (top 48 pixels)
    u8g2.setDrawColor(0);                // Erase mode
    u8g2.drawBox(0, 0, 128, 48);         // x, y, width, height
    u8g2.setDrawColor(1);                // Restore draw mode
}

//this function sends all stored things in the buffer to the display
void outputOnDisplay(){
    u8g2.sendBuffer();
}


//This is just a helper function for drawing the clock hands
//time is either the minutes or the hours in seconds!, depending on which hand is supposed to be drawn
void drawClockHands(float angle, int length, int centerX, int centerY){
    int x1 = centerX + sin(angle) * length;  // Use sin for x
    int y1 = centerY - cos(angle) * length;  // Use cos for y and invert for screen
    u8g2.drawLine(centerX, centerY, x1, y1);
}