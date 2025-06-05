#include <U8g2lib.h>

extern int TZ_MENU_INDEX;

void createInitialMenu();
void enterPressed();
void handleSelection();
void displayMenu();
void forwardMenu();
void backwardMenu();
void clearPreviousMenuText();
void displayTimeZoneMenu();
int getTzIndex();
