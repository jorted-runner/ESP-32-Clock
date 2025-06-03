#include <Arduino.h>
#include "Menu.h"
#include "OutputOnDisplay.h"

#include <iostream>
#include <vector>
#include <string>

std::vector<std::string> menuOptions = {"Timezone", "Timer", "Exit"};
int MENU_INDEX = 0;
bool MENU_ENTERED = false;

void createInitialMenu() {

    u8g2.setFont(u8g2_font_profont15_tr);
    u8g2.drawStr(6, 58, "Menu");
    u8g2.drawLine(4, 60, 35, 60);
}

void enterPressed() {
    if (!MENU_ENTERED) {
        MENU_ENTERED = true;
        enterMenu();
    } else {
        handleSelection();
    }
}

void handleSelection() {
    if (MENU_INDEX == 0) {
        // create timezone menu
        Serial.println(menuOptions[MENU_INDEX].c_str());
    } else if (MENU_INDEX == 1) {
        // create timer menu or start timer
        Serial.println(menuOptions[MENU_INDEX].c_str());
    } else if (MENU_INDEX == 2) {
        MENU_INDEX = 0;
        MENU_ENTERED = false;
        clearPreviousMenuText();
        createInitialMenu();
    } 
}

void enterMenu() {
    u8g2.setFont(u8g2_font_profont10_tr);
    clearPreviousMenuText();
    int x = 6;
    int y = 58;

    for (size_t i = 0; i < menuOptions.size(); ++i) {
        const char* label = menuOptions[i].c_str();
        u8g2.drawStr(x, y, label);

        if (i == MENU_INDEX) {
            int strWidth = u8g2.getStrWidth(label);
            u8g2.drawLine(x, y + 2, x + strWidth, y + 2);  // underline
        }

        x += u8g2.getStrWidth(label) + 10;  // spacing between words
    }
}

void forwardMenu() {
    if (MENU_INDEX + 1 > menuOptions.size() - 1) {
        MENU_INDEX = 0;
    } else {
        MENU_INDEX += 1;
    }
    enterMenu();
}

void backwardMenu() {
    if (MENU_INDEX - 1 < 0) {
        MENU_INDEX = menuOptions.size() - 1;
    } else {
        MENU_INDEX -= 1;
    }
    enterMenu();
}

void clearPreviousMenuText() {
    u8g2.setDrawColor(0);
    // x, y, w, h
    u8g2.drawBox(0, 48, 128, 16); // Clear previous text (adjust width/height as needed)
    u8g2.setDrawColor(1);

}