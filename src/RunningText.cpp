#include "RunningText.h"

int* runningBuffer;
int bufferWidth;
int position;
int timerInterval = 50;

os_timer_t updateTimer;
bool running = false;

/**
 * Puts a symbol in the running buffer so it can be displayed later.
 *
 * Parameter x: How far to the right the symbol should be placed in pixels.
 * Parameter arr: Array containing the symbol, see MatrixConstants.h file for examples.
 * Parameter len: Width of the symbol in pixels.
 */
void symbolInRunningBuffer(int x, int* arr, int len) {
    for (int i = 0; i < len; i++) {
        runningBuffer[x + i] = *(arr + i);
    }
}

/**
* Shifts the whole text one pixel to the left.
* Stops when the whole text has been displayed.
*/
void updatePosition(void *pArg) {
    if (position < (bufferWidth - (MAX_IN_USE * 8))) {
        Matrix::setWholeBuffer(runningBuffer + position);
        Matrix::drawBuffer();
        position++;
    }
    else {
        os_timer_disarm(&updateTimer);
        running = false;
    }
}

/**
 * Sets the text that should be displayed.
 *
 * Parameter text: The text that should be displayed.
 */
void RunningText::setText(String text) {
    stop();
    bufferWidth = (text.length() * 6) + (MAX_IN_USE * 16);
    delete[] runningBuffer;
    runningBuffer = new int [bufferWidth];

    int pos = 0;
    symbolInRunningBuffer(pos, empty, MAX_IN_USE * 8);
    pos += (MAX_IN_USE * 8);

    for (unsigned int i = 0; i < text.length(); i++) {
        symbolInRunningBuffer(pos, letters[text[i] - 32], 5);
        pos += 5;
        runningBuffer[pos] = 0;
        pos++;
    }

    symbolInRunningBuffer(pos, empty, MAX_IN_USE * 8);

    position = 0;

    os_timer_setfn(&updateTimer, updatePosition, NULL);
    os_timer_arm(&updateTimer, timerInterval, true);
    running = true;
}

/**
 * Sets the time interval between the steps the text takes on the display.
 * 
 * Parameter interval: The time interval in milliseconds.
 * Returns: 0 on success, 1 when the time interval is too low.
 */
int RunningText::setInterval(int interval) {

    // An interval lower than 50 causes problems
    if (interval < 50)
        return 1;

    timerInterval = interval;
    os_timer_disarm(&updateTimer);
    os_timer_arm(&updateTimer, timerInterval, true);
    running = true;

    return 0;
}

/**
 * Stops the running text and clears the display.
 */
void RunningText::stop() {
    os_timer_disarm(&updateTimer);
    running = false;
    Matrix::clearAll();
}

/**
 * Checks, if the display is currently displaying a running text.
 */
bool RunningText::isRunning() {
    return running;
}
