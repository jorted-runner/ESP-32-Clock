#include <U8g2lib.h>

extern int TZ_MENU_INDEX;
extern int DST_MENU_INDEX;
extern int TIME_FORMAT_MENU_INDEX;

static const unsigned char image_check_bits[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x0c,0x00,0x06,0x01,0x03,0x83,0x01,0xc6,0x00,0x6c,0x00,0x38,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
static const unsigned char image_cross_small_bits[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x03,0x86,0x01,0xcc,0x00,0x78,0x00,0x30,0x00,0x78,0x00,0xcc,0x00,0x86,0x01,0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x00};

void createInitialMenu();
void enterPressed();
void handleSelection();
void displayMainMenu();
void forwardMenu();
void backwardMenu();
void clearPreviousMenuText();
void displayTimeZoneMenu();
void displayDstMenu();
int getTzIndex();
int getDstIndex();
void initiateTimer();
void displayTimer();
void displayTimeFormatMenu();
bool getTimeFormat();
void prepMenu();
