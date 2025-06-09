#include "TimerUtils.h"

#include <iostream>
#include <Arduino.h>

TimerState timer;

void formatElapsedTime(unsigned long elapsed, char* buffer) {
    int hours = elapsed / 3600000;
    int minutes = (elapsed / 60000) % 60;
    int seconds = (elapsed / 1000) % 60;
    sprintf(buffer, "%02d:%02d:%02d", hours, minutes, seconds);
}

void initiateTimer() {
    timer.startMillis = millis();
    timer.running = true;
}

void toggleTimer() {
    if (isTimerRunning()) {
        pauseTimer();
    } else {
        resumeTimer();
    }
}

void pauseTimer() {
    if (isTimerRunning()) {
        timer.pausedTime += millis() - timer.startMillis;
        timer.running = false;
    }
}

void resumeTimer() {
    if (!isTimerRunning()) {
        timer.startMillis = millis();
        timer.running = true;
    }
}

void restartTimer() {
    timer.pausedTime = 0;
    timer.startMillis = millis();
    timer.running = true;
}

void resetTimer() {
    timer.running = false;
    timer.startMillis = 0;
    timer.pausedTime = 0;
}

bool isTimerRunning() {
    return timer.running;
}

unsigned long getElapsedTime() {
    unsigned long elapsed = timer.pausedTime;
    if (isTimerRunning()) {
        elapsed += millis() - timer.startMillis;
    }
    return elapsed;
}