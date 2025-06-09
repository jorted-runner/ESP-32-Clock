struct TimerState {
    bool running = false;
    unsigned long startMillis = 0;
    unsigned long pausedTime = 0;
};

extern TimerState timer;

void formatElapsedTime(unsigned long elapsed, char* buffer);
void initiateTimer();
void resetTimer();
void toggleTimer();
void pauseTimer();
void resumeTimer();
void restartTimer();
bool isTimerRunning();
unsigned long getElapsedTime();