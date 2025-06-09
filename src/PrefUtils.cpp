#include "PreferencesGlobals.h"
#include "PrefUtils.h"

int getUtcOffset() {
    clockPrefs.begin("clockPrefs", RO_MODE);
    int offset = clockPrefs.getInt("utcOff", 0);
    clockPrefs.end();
    return offset;
}

int getUtcOffsetInSeconds() {
    int offset = getUtcOffset() * 3600;
    return offset;
}

int getDstOffsetInSeconds() {
    int offset = getDstOffset() * 3600;
    return offset;
}

int getDstOffset() {
    clockPrefs.begin("clockPrefs", RO_MODE);
    int offset = clockPrefs.getInt("dstOff", 0);
    clockPrefs.end();
    return offset;
}

bool getTimeFormat() {
    clockPrefs.begin("clockPrefs", RO_MODE);
    int timeFormat = clockPrefs.getBool("24hour", true);
    clockPrefs.end();
    return timeFormat;
}

bool checkKeyExists(char* key) {
    clockPrefs.begin("clockPrefs", RO_MODE);
    return clockPrefs.isKey(key);
}

void setIntPref(char* key, int value) {
    clockPrefs.begin("clockPrefs", RW_MODE);
    clockPrefs.putInt(key, value);
    clockPrefs.end();
}

void setBoolPref(char* key, bool value) {
    clockPrefs.begin("clockPrefs", RW_MODE);
    clockPrefs.putBool(key, value);
    clockPrefs.end();
}