#include "TimerUtils.h"

#include <iostream>
#include <Arduino.h>

TimerState timer;

// Formats elapsed time (in milliseconds) into HH:MM:SS string.
void formatElapsedTime(unsigned long elapsed, char* buffer) {
    int hours = elapsed / 3600000;
    int minutes = (elapsed / 60000) % 60;
    int seconds = (elapsed / 1000) % 60;
    sprintf(buffer, "%02d:%02d:%02d", hours, minutes, seconds);
}

// Starts the timer by recording the current millis() timestamp.
void initiateTimer() {
    timer.startMillis = millis();
    timer.running = true;
}

// Toggles timer state between running and paused.
void toggleTimer() {
    if (isTimerRunning()) {
        pauseTimer();
    } else {
        resumeTimer();
    }
}

// Pauses the timer and accumulates elapsed time.
void pauseTimer() {
    if (isTimerRunning()) {
        timer.pausedTime += millis() - timer.startMillis;
        timer.running = false;
    }
}

// Resumes the timer from paused state.
void resumeTimer() {
    if (!isTimerRunning()) {
        timer.startMillis = millis();
        timer.running = true;
    }
}

// Restarts the timer from zero and sets running state.
void restartTimer() {
    timer.pausedTime = 0;
    timer.startMillis = millis();
    timer.running = true;
}

// Resets the timer to initial state and stops it.
void resetTimer() {
    timer.running = false;
    timer.startMillis = 0;
    timer.pausedTime = 0;
}

// Returns true if the timer is currently running.
bool isTimerRunning() {
    return timer.running;
}

// Returns the total elapsed time in milliseconds.
unsigned long getElapsedTime() {
    unsigned long elapsed = timer.pausedTime;
    if (isTimerRunning()) {
        elapsed += millis() - timer.startMillis;
    }
    return elapsed;
}