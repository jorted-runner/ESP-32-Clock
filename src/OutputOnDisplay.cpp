#include <Arduino.h>
#include "OutputOnDisplay.h"

//Depending on your screen, you need to uncomment either one of them. Just try out, which one works, 
//in case you arent sure which one you need
//In case you know which driver you are using (sh1106 or ssd1306) you just need to uncomment that one and comment
//out the other one.

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, SCL_PIN, SDA_PIN, RESET_PIN);
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

    //this part is used to take the timeinfo, and convert the hours and minutes to an array of chars
    //this is done, since the u8g2.drawStr method is expecting it to be of that type.
    char timeStr[6]; //HH:MM + null terminator
    snprintf(timeStr, sizeof(timeStr), "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
    
    //here we set the font
    u8g2.setFont(u8g2_font_profont22_tr);

    //this is where we actually write the time to the buffer
    u8g2.drawStr(59, 20, timeStr);
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

//this function sends all stored things in the buffer to the display
void outputOnDisplay(){
    u8g2.sendBuffer();
}