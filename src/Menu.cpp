#include "Menu.h"
#include "OutputOnDisplay.h"
#include "PreferencesGlobals.h"
#include "TimeAPICall.h"
#include "PrefUtils.h"
#include "TimerUtils.h"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <Arduino.h>

// ----------------------------------------------------------------
// Menu option definitions and selection indices
// mainMenuOptions: labels for each main menu item
// tzMenuOptions: list of UTC offsets for timezone selection
// dstMenuOptions: options for toggling Daylight Savings
// timeFormatMenuOptions: options for 12/24 hour display format
// Index variables track current selection in each menu context
// ----------------------------------------------------------------
std::vector<std::string> mainMenuOptions = {"Timezone", "Timer", "TimeFormat", "Exit"};
std::vector<int> tzMenuOptions = {-12, -11, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
std::vector<std::string> dstMenuOptions = {"F", "T"};
std::vector<std::string> timeFormatMenuOptions = {"12 hour", "24 hour"};

int MAIN_MENU_INDEX = 0;
int TZ_MENU_INDEX;
int DST_MENU_INDEX;
int TIME_FORMAT_MENU_INDEX;

bool MAIN_MENU_ENTERED = false;
bool TZ_MENU_ENTERED = false;
bool DST_MENU_ENTERED = false;
bool TIMER_ENTERED = false;
bool TIME_FORMAT_ENTERED = false;

int initialX = 6;
int initialY = 58;

// Returns the index in tzMenuOptions matching the stored UTC offset preference.
int getTzIndex() {
    int offSet = getUtcOffset();

    // Search the vector for the offset retrieved from the Preferences
    auto it = std::find(tzMenuOptions.begin(), tzMenuOptions.end(), offSet);

    // If the offset is found return the index, otherwise return 12
    if (it != tzMenuOptions.end()) {
        return static_cast<int>(std::distance(tzMenuOptions.begin(), it));
    } else {
        return 12; // Index for UTC 0
    }
}

// Returns the stored Daylight Savings Time preference index (0 or 1).
int getDstIndex() {
    int offSet = getDstOffset(); // Default to 0
    return offSet;
}

// Draws the initial menu title ("Menu") and underline on the display.
void createInitialMenu() {
    u8g2.setFont(u8g2_font_profont15_tr);
    u8g2.drawStr(6, 58, "Menu");
    u8g2.drawLine(4, 60, 35, 60); // Underline Menu
}

// Handles Enter button: enters the main menu or processes the current selection.
void enterPressed() {
    if (!MAIN_MENU_ENTERED) { // If main menu not entered -> Display the main menu
        MAIN_MENU_ENTERED = true;
        displayMainMenu();
    } else {
        handleSelection(); // if the main menu has already been entered, handle menu item selection
    }
}

// Processes the selected menu item, updates user preferences, and navigates between submenus.
void handleSelection() {
    if (DST_MENU_ENTERED) {  // If in DST menu Clock needs to be updated to reflect users new Timezone and Daylight Savings preferences
        DST_MENU_ENTERED = false;
        setIntPref("dstOff", DST_MENU_INDEX);

        int utcOffset = getUtcOffsetInSeconds();
        int dstOffset = getDstOffsetInSeconds();

        configTime(utcOffset, dstOffset, NTP_SERVER);

        displayMainMenu();
    } else if (TZ_MENU_ENTERED) { // If in Timezone menu, set users timezone preference to the selected preference
        TZ_MENU_ENTERED = false;
        setIntPref("utcOff", tzMenuOptions[TZ_MENU_INDEX]);
        DST_MENU_ENTERED = true;
        displayDstMenu();
    } else if (TIMER_ENTERED) { // If in timer, return to main menu and reset timer settings
        TIMER_ENTERED = false;
        resetTimer();
        displayMainMenu();
    } else if (TIME_FORMAT_ENTERED) { // If in time format menu, store user selected time format preference
        TIME_FORMAT_ENTERED = false;
        setBoolPref("24hour", TIME_FORMAT_MENU_INDEX);
        displayMainMenu();
    } else {
        if (MAIN_MENU_INDEX == 0) {
            // enter timezone menu
            TZ_MENU_ENTERED = true;
            displayTimeZoneMenu();
        } else if (MAIN_MENU_INDEX == 1) {
            // initiate timer
            TIMER_ENTERED = true;
            initiateTimer();
        } else if (MAIN_MENU_INDEX == 2) {
            //enter time format menu
            TIME_FORMAT_ENTERED = true;
            displayTimeFormatMenu();
        } else if (MAIN_MENU_INDEX == 3) {
            // Exit main menu
            MAIN_MENU_INDEX = 0;
            MAIN_MENU_ENTERED = false;
            clearPreviousMenuText();
            createInitialMenu();
        }
    }
}

// Handles Forward button: moves selection forward within the active menu
void forwardMenu() {
    if (TZ_MENU_ENTERED) { // Move forward in the Timezone menu
        if (TZ_MENU_INDEX + 1 > tzMenuOptions.size() - 1) {
            TZ_MENU_INDEX = tzMenuOptions.size() - 1;
        } else {
            TZ_MENU_INDEX += 1;
        }
        displayTimeZoneMenu();
    } else if (DST_MENU_ENTERED) { // Move forward in the Daylight Savings Menu
        if (DST_MENU_INDEX == 1) {
            DST_MENU_INDEX = 0;
        } else {
            DST_MENU_INDEX = 1;
        }
        displayDstMenu();
    } else if (TIMER_ENTERED) {
        toggleTimer(); // Pause/start timer
    } else if (TIME_FORMAT_ENTERED) { // Move forward in the time format menu
        if (TIME_FORMAT_MENU_INDEX == 1) {
            TIME_FORMAT_MENU_INDEX = 0;
        } else {
            TIME_FORMAT_MENU_INDEX = 1;
        }
        displayTimeFormatMenu();
    } else if (MAIN_MENU_ENTERED) { // Move forward in the Main Menu
        if (MAIN_MENU_INDEX + 1 > mainMenuOptions.size() - 1) {
            MAIN_MENU_INDEX = 0;
        } else {
            MAIN_MENU_INDEX += 1;
        }
        displayMainMenu();
    }
}

// Handles Backward button: moves selection backward within the active menu
void backwardMenu() {
    if (TZ_MENU_ENTERED) { // Move backward in Timezone menu
        if (TZ_MENU_INDEX - 1 < 0) {
            TZ_MENU_INDEX = 0;
        } else {
            TZ_MENU_INDEX -= 1;
        }
        displayTimeZoneMenu();
    } else if (DST_MENU_ENTERED) { // Move backward in Daylight savings menu
        if (DST_MENU_INDEX == 0) {
            DST_MENU_INDEX = 1;
        } else {
            DST_MENU_INDEX = 0;
        }
        displayDstMenu();
    } else if (TIMER_ENTERED) { // Restart Timer
        restartTimer();
    } else if (TIME_FORMAT_ENTERED) { // Move backward in Time format menu
        if (TIME_FORMAT_MENU_INDEX == 0) {
            TIME_FORMAT_MENU_INDEX = 1;
        } else {
            TIME_FORMAT_MENU_INDEX = 0;
        }
        displayTimeFormatMenu();
    } else if (MAIN_MENU_ENTERED) { // Move backward in Main Menu
        if (MAIN_MENU_INDEX - 1 < 0) {
            MAIN_MENU_INDEX = mainMenuOptions.size() - 1;
        } else {
            MAIN_MENU_INDEX -= 1;
        }
        displayMainMenu();
    }
}

// Renders two main menu items centered around MAIN_MENU_INDEX and underlines the selected one.
void displayMainMenu() {
    prepMenu();

    // set initial x, y
    int x = initialX;
    int y = initialY;

    const int displayCount = 2;  // show 2 items at a time
    int total = (int)mainMenuOptions.size();
    int start;

    // set starting index based off current menu index
    if (MAIN_MENU_INDEX == 0) {
        start = 0;
    } else if (MAIN_MENU_INDEX >= total - 1) {
        start = total - 2;
        if (start < 0) start = 0;
    } else {
        start = MAIN_MENU_INDEX - 1;
    }

    // display required number of menu items and underline current menu index
    for (int idx = start; idx < start + displayCount && idx < total; ++idx) {
        const char* label = mainMenuOptions[idx].c_str();
        u8g2.drawStr(x, y, label);

        if (idx == MAIN_MENU_INDEX) {
            int strWidth = u8g2.getStrWidth(label);
            u8g2.drawLine(x, y + 2, x + strWidth, y + 2);  // underline selected
        }

        x += u8g2.getStrWidth(label) + 10; // calculate starting x based of labels width plus spaceing
    }
}

// Renders timezone options centered around TZ_MENU_INDEX with a "UTC:" label and underlines selection.
void displayTimeZoneMenu() {
    prepMenu();

    // set initial x, y
    int x = initialX;
    int y = initialY;

    // create and draw UTC label
    const char* utcLabel = "UTC:";
    u8g2.drawStr(x, y, utcLabel);
    x+= u8g2.getStrWidth(utcLabel) + 10;
   
    // Set menu items to display and find middle
    const int displayCount = 5;
    const int half = displayCount / 2;

    int total = (int)tzMenuOptions.size();
    int start;

    // set index for first time to display
    if (TZ_MENU_INDEX < half) {
        start = 0;
    } else if (TZ_MENU_INDEX > total - half - 1) {
        start = total - displayCount;
    } else {
        start = TZ_MENU_INDEX - half;
    }

    // display required number of menu items and underline current menu index
    for (int idx = start; idx < start + displayCount; ++idx) {
        std::string labelStr = std::to_string(tzMenuOptions[idx]);
        const char*  label   = labelStr.c_str();

        u8g2.drawStr(x, y, label);

        if (idx == TZ_MENU_INDEX) {
            int strWidth = u8g2.getStrWidth(label);
            u8g2.drawLine(x, y + 2, x + strWidth, y + 2);  // underline selection
        }

        x += u8g2.getStrWidth(label) + 5; // calculate starting x based of labels width plus spaceing
    }
}

// Renders Daylight Savings toggle options and underlines the selected state.
void displayDstMenu() {
    prepMenu();

    int x = initialX;
    int y = initialY;

    const char* utcLabel = "Daylight Savings:";
    u8g2.drawStr(x, y, utcLabel);
    x+= u8g2.getStrWidth(utcLabel) + 10;

    int crossWidth = 10;
    int checkWidth = 12;

    y = 46;

    for (size_t i = 0; i < dstMenuOptions.size(); ++i) {
        const char* label = dstMenuOptions[i].c_str();
        //u8g2.drawStr(x, y, label);
        if (i == 0){
            u8g2.drawXBM(x, y, crossWidth, 16, image_cross_small_bits);
            if (i == DST_MENU_INDEX) {
                int strWidth = u8g2.getStrWidth(label);
                u8g2.drawLine(x, y + 14, x + crossWidth, y + 14);  // underline
            }
        } else {
            u8g2.drawXBM(x, y, checkWidth, 16, image_check_bits);
            if (i == DST_MENU_INDEX) { 
                int strWidth = u8g2.getStrWidth(label);
                u8g2.drawLine(x, y + 14, x + checkWidth, y + 14);  // underline
            }
        }

        x += u8g2.getStrWidth(label) + 10;  // spacing between words
    }
}

// Displays the running timer (HH:MM:SS) if the timer is active.
void displayTimer() {
    if (TIMER_ENTERED && isTimerRunning()) {
        unsigned long elapsed = getElapsedTime();

        char buffer[9];
        formatElapsedTime(elapsed, buffer);
        prepMenu();
        u8g2.drawStr(initialX, initialY, buffer);
        u8g2.sendBuffer();
    }
}

// Renders time format options ("12 hour", "24 hour") and underlines the selected format.
void displayTimeFormatMenu() {
    prepMenu();
    int x = initialX;
    int y = initialY;

    const char* utcLabel = "Format:";
    u8g2.drawStr(x, y, utcLabel);
    x+= u8g2.getStrWidth(utcLabel) + 10;

    for (size_t i = 0; i < timeFormatMenuOptions.size(); ++i) {
        const char* label = timeFormatMenuOptions[i].c_str();
        u8g2.drawStr(x, y, label);

        if (i == TIME_FORMAT_MENU_INDEX) {
            int strWidth = u8g2.getStrWidth(label);
            u8g2.drawLine(x, y + 2, x + strWidth, y + 2);  // underline
        }

        x += u8g2.getStrWidth(label) + 10;  // spacing between words
    }
}

// Clears the bottom portion of the display buffer where menu text is drawn.
void clearPreviousMenuText() {
    u8g2.setDrawColor(0);
    // x, y, w, h
    u8g2.drawBox(0, 48, 128, 16); // Clear previous text
    u8g2.setDrawColor(1);
}

// Prepares display settings: selects the font and clears previous menu area for redrawing.
void prepMenu() {
    u8g2.setFont(u8g2_font_profont10_tr);
    clearPreviousMenuText();
} 