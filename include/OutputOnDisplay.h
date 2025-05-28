#ifndef OUTPUT_ON_DISPLAY_H
#define OUTPUT_ON_DISPLAY_H

#include <U8g2lib.h>

//these are all the display related constants
//these are the pins used for connection between esp32 and display
#define SDA_PIN 5
#define SCL_PIN 6
//this defines that there is no reset pin being used
#define RESET_PIN U8X8_PIN_NONE

extern U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2;


//here are all the functions declarations
void prepareDisplay();
void sendTimeToDisplayBuffer();
void outputOnDisplay();

#endif