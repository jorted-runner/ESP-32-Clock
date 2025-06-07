#include <Arduino.h>
#include "Menu.h"
#include "OutputOnDisplay.h"
#include "PreferencesGlobals.h"
#include "TimeAPICall.h"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

std::vector<std::string> mainMenuOptions = {"Timezone", "Timer", "Clock Options","Exit"};
std::vector<int> tzMenuOptions = {-12, -11, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
std::vector<std::string> dstMenuOptions = {"F", "T"};

int MAIN_MENU_INDEX = 0;
int TZ_MENU_INDEX;
int DST_MENU_INDEX;

bool MAIN_MENU_ENTERED = false;
bool TZ_MENU_ENTERED = false;
bool DST_MENU_ENTERED = false;

int getTzIndex() {
    clockPrefs.begin("clockPrefs", false);
    int offSet = clockPrefs.getInt("utcOff", 0);
    clockPrefs.end();

    // Search the vector for the offset retrieved from the Preferences
    auto it = std::find(tzMenuOptions.begin(), tzMenuOptions.end(), offSet);

    // If the offset is found return the index, otherwise return 12
    if (it != tzMenuOptions.end()) {
        return static_cast<int>(std::distance(tzMenuOptions.begin(), it));
    } else {
        return 12;
    }
}

int getDstIndex() {
    clockPrefs.begin("clockPrefs", false);
    int offSet = clockPrefs.getInt("dstOff", 0);
    clockPrefs.end();
    return offSet;
}

void createInitialMenu() {
    u8g2.setFont(u8g2_font_profont15_tr);
    u8g2.drawStr(6, 58, "Menu");
    u8g2.drawLine(4, 60, 35, 60);
}

void enterPressed() {
    if (!MAIN_MENU_ENTERED) {
        MAIN_MENU_ENTERED = true;
        displayMenu();
    } else {
        handleSelection();
    }
}

void handleSelection() {
    if (DST_MENU_ENTERED) {
        DST_MENU_ENTERED = false;
        clockPrefs.begin("clockPrefs", false);
        clockPrefs.putInt("dstOff", DST_MENU_INDEX);

        int utcOffset = clockPrefs.getInt("utcOff", 0) * 3600;
        int dstOffset = clockPrefs.getInt("dstOff", 0) * 3600;
        configTime(utcOffset, dstOffset, NTP_SERVER);

        clockPrefs.end();
        displayMenu();
    } else if (TZ_MENU_ENTERED) {
        TZ_MENU_ENTERED = false;
        clockPrefs.begin("clockPrefs", false);
        clockPrefs.putInt("utcOff", tzMenuOptions[TZ_MENU_INDEX]);
        clockPrefs.end();
        DST_MENU_ENTERED = true;
        displayDstMenu();
    } else {
        if (MAIN_MENU_INDEX == 0) {
            TZ_MENU_ENTERED = true;
            displayTimeZoneMenu();
        } else if (MAIN_MENU_INDEX == 1) {
            // create timer menu or start timer
            Serial.println(mainMenuOptions[MAIN_MENU_INDEX].c_str());
        } else if (MAIN_MENU_INDEX == 2) {
            // clock display option 12 or 24 hour


            
        } else if (MAIN_MENU_INDEX == 3) {
            MAIN_MENU_INDEX = 0;
            MAIN_MENU_ENTERED = false;
            clearPreviousMenuText();
            createInitialMenu();
        }
    }

}

void displayMenu() {
    u8g2.setFont(u8g2_font_profont10_tr);
    clearPreviousMenuText();
    int x = 6;
    int y = 58;

    for (size_t i = 0; i < mainMenuOptions.size(); ++i) {
        const char* label = mainMenuOptions[i].c_str();
        u8g2.drawStr(x, y, label);

        if (i == MAIN_MENU_INDEX) {
            int strWidth = u8g2.getStrWidth(label);
            u8g2.drawLine(x, y + 2, x + strWidth, y + 2);  // underline
        }

        x += u8g2.getStrWidth(label) + 10;  // spacing between words
    }
}

void displayTimeZoneMenu() {
    u8g2.setFont(u8g2_font_profont10_tr);
    clearPreviousMenuText();
    int x = 6;
    int y = 58;

    const char* utcLabel = "UTC:";
    u8g2.drawStr(x, y, utcLabel);
    x+= u8g2.getStrWidth(utcLabel) + 10;

    const int displayCount = 5;
    const int half = displayCount / 2;

    int total = (int)tzMenuOptions.size();
    int start;

    if (TZ_MENU_INDEX < half) {
        start = 0;
    } else if (TZ_MENU_INDEX > total - half - 1) {
        start = total - displayCount;
    } else {
        start = TZ_MENU_INDEX - half;
    }

    for (int idx = start; idx < start + displayCount; ++idx) {
        std::string labelStr = std::to_string(tzMenuOptions[idx]);
        const char*  label   = labelStr.c_str();

        u8g2.drawStr(x, y, label);

        if (idx == TZ_MENU_INDEX) {
            int strWidth = u8g2.getStrWidth(label);
            u8g2.drawLine(x, y + 2, x + strWidth, y + 2);  // underline
        }

        x += u8g2.getStrWidth(label) + 5;
    }
}

void displayDstMenu() {
    u8g2.setFont(u8g2_font_profont10_tr);
    clearPreviousMenuText();
    int x = 6;
    int y = 58;

    const char* utcLabel = "Daylight Savings:";
    u8g2.drawStr(x, y, utcLabel);
    x+= u8g2.getStrWidth(utcLabel) + 10;

    for (size_t i = 0; i < dstMenuOptions.size(); ++i) {
        const char* label = dstMenuOptions[i].c_str();
        u8g2.drawStr(x, y, label);

        if (i == DST_MENU_INDEX) {
            int strWidth = u8g2.getStrWidth(label);
            u8g2.drawLine(x, y + 2, x + strWidth, y + 2);  // underline
        }

        x += u8g2.getStrWidth(label) + 10;  // spacing between words
    }
}

void forwardMenu() {
    if (TZ_MENU_ENTERED) {
        if (TZ_MENU_INDEX + 1 > tzMenuOptions.size() - 1) {
            TZ_MENU_INDEX = tzMenuOptions.size() - 1;
        } else {
            TZ_MENU_INDEX += 1;
        }
        displayTimeZoneMenu();
    } else if (DST_MENU_ENTERED) {
        if (DST_MENU_INDEX == 1) {
            DST_MENU_INDEX = 0;
        } else {
            DST_MENU_INDEX = 1;
        }
        displayDstMenu();
    } else if (MAIN_MENU_ENTERED) {
        if (MAIN_MENU_INDEX + 1 > mainMenuOptions.size() - 1) {
            MAIN_MENU_INDEX = 0;
        } else {
            MAIN_MENU_INDEX += 1;
        }
        displayMenu();
    }
}

void backwardMenu() {
    if (TZ_MENU_ENTERED) {
        if (TZ_MENU_INDEX - 1 < 0) {
            TZ_MENU_INDEX = 0;
        } else {
            TZ_MENU_INDEX -= 1;
        }
        displayTimeZoneMenu();
    } else if (DST_MENU_ENTERED) {
        if (DST_MENU_INDEX == 0) {
            DST_MENU_INDEX = 1;
        } else {
            DST_MENU_INDEX = 0;
        }
        displayDstMenu();
    } else if (MAIN_MENU_ENTERED) {
        if (MAIN_MENU_INDEX - 1 < 0) {
            MAIN_MENU_INDEX = mainMenuOptions.size() - 1;
        } else {
            MAIN_MENU_INDEX -= 1;
        }
        displayMenu();
    }
}

void clearPreviousMenuText() {
    u8g2.setDrawColor(0);
    // x, y, w, h
    u8g2.drawBox(0, 48, 128, 16); // Clear previous text
    u8g2.setDrawColor(1);

}