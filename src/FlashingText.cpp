#include "FlashingText.h"

int wordCount;
String* flashingBuffer;
int currentText;
int displayDuration = 2000;
bool flashing = false;
int iterationsLeft = 0;
bool stopNextTime = false;

/**
 * Displays the provided text in the center of the display.
 * 
 * Parameter text: The text that should be displayed.
 */
void setCenteredText(String text) {
    int ledLength = text.length() * 6 - 1;
    if (ledLength > MAX_IN_USE * 8)
        return;

    int *bufferContent = new int[ledLength - 1]();
    for (unsigned int i = 0; i < text.length(); i++) {
        for (int j = 0; j < 5; j++) {
            bufferContent[i*6 + j] = letters[text[i] - 32][j];
        }
    }

    int startPosition = (MAX_IN_USE * 8 - ledLength) / 2;

    Matrix::symbolInBuffer(startPosition, bufferContent, ledLength);
    delete[] bufferContent;
    Matrix::drawBuffer();
}

/**
 * Changes the displayed text to the next text in the buffer.
 */
void FlashingText::updateCurrentText() {
    if (stopNextTime) {
        FlashingText::stop();
        return;
    }
    setCenteredText(flashingBuffer[currentText]);
    currentText++;
    if (currentText >= wordCount) {
        if (iterationsLeft == 1)
            stopNextTime = true;
        else if (iterationsLeft > 1)
            iterationsLeft--;
        currentText = 0;
    }
}

/**
 * Sets the text that should be displayed.
 *
 * Parameter text: The text that should be displayed.
 * Parameter iterations: The number of times the text should be displayed or 0 for infinite times.
 * Returns: 0 on success, 1 when a word exceeds the maximum length of the display.
 */
int FlashingText::setText(String text, int iterations) {
    stop();
    RunningText::stop();

    if(text.length() < 1)
        return 1;
    
    wordCount = 1;
    for (unsigned int i = 0; i < text.length(); i++) {
        if (text[i] == ';')
            wordCount++;
    }

    iterationsLeft = iterations;
    stopNextTime = false;
    
    delete[] flashingBuffer;
    flashingBuffer = new String [wordCount];

    int lastSemiColon = -1;
    int wordIndex = 0;
    for (unsigned int i = 0; i < text.length(); i++) {
        if (text[i] == ';') {
            String singleWord = text.substring(lastSemiColon+1, i);

            // The words must not exceed the maximum width of the display
            int ledLength = singleWord.length() * 6 - 1;
            if (ledLength > MAX_IN_USE * 8)
                return 1;

            flashingBuffer[wordIndex] = singleWord;

            lastSemiColon = i;
            wordIndex++;
        }
    }

    String singleWord = text.substring(lastSemiColon+1);

    // The words must not exceed the maximum width of the display
    int ledLength = singleWord.length() * 6 - 1;
    if (ledLength > MAX_IN_USE * 8)
        return 1;

    flashingBuffer[wordIndex] = singleWord;

    currentText = 0;

    SchedulerUtils::enableFlashingText();
    flashing = true;

    return 0;
}

/**
 * Sets the time each text fragment is displayed.
 * 
 * Parameter interval: The time period in milliseconds.
 * Returns: 0 on success, 1 when the time interval is too low.
 */
int FlashingText::setInterval(int interval) {
    // An interval lower than 300 makes no sense
    if (interval < 300)
        return 1;

    displayDuration = interval;
    SchedulerUtils::enableFlashingText();
    flashing = true;

    return 0;
}

/**
 * Stops the flashing text and clears the display.
 */
void FlashingText::stop() {
    SchedulerUtils::disableFlashingText();
    flashing = false;
    Matrix::clearAll();
}

/**
 * Checks, if the display is currently displaying a flashing text.
 */
bool FlashingText::isFlashing() {
    return flashing;
}
