#include "PreferencesGlobals.h"
#include "PrefUtils.h"

// Reads the stored UTC offset preference (defaults to 0).
int getUtcOffset() {
    clockPrefs.begin("clockPrefs", RO_MODE);
    int offset = clockPrefs.getInt("utcOff", 0);
    clockPrefs.end();
    return offset;
}

// Returns the stored UTC offset preference in seconds.
int getUtcOffsetInSeconds() {
    int offset = getUtcOffset() * 3600;
    return offset;
}

// Returns the stored DST offset preference in seconds.
int getDstOffsetInSeconds() {
    int offset = getDstOffset() * 3600;
    return offset;
}

// Reads the stored Daylight Savings Time offset preference (defaults to 0).
int getDstOffset() {
    clockPrefs.begin("clockPrefs", RO_MODE);
    int offset = clockPrefs.getInt("dstOff", 0);
    clockPrefs.end();
    return offset;
}

// Reads the stored time format preference (true for 24-hour, false for 12-hour) from NVS.
bool getTimeFormat() {
    clockPrefs.begin("clockPrefs", RO_MODE);
    int timeFormat = clockPrefs.getBool("24hour", true);
    clockPrefs.end();
    return timeFormat;
}

// Checks if a given key exists in the preferences storage.
bool checkKeyExists(const char* key) {
    clockPrefs.begin("clockPrefs", RO_MODE);
    return clockPrefs.isKey(key);
}

// Writes a integer preference value to preferences under the given key.
void setIntPref(const char* key, int value) {
    clockPrefs.begin("clockPrefs", RW_MODE);
    clockPrefs.putInt(key, value);
    clockPrefs.end();
}

// Writes a boolean preference value to preferences under the given key.
void setBoolPref(const char* key, bool value) {
    clockPrefs.begin("clockPrefs", RW_MODE);
    clockPrefs.putBool(key, value);
    clockPrefs.end();
}