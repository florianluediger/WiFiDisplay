#include "FlashingText.h"

int wordCount;
String* flashingBuffer;
int currentText;
int displayDuration = 2000;

os_timer_t interruptTimer;

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

void updateCurrentText(void *pArg) {
    setCenteredText(flashingBuffer[currentText]);
    currentText++;
    if (currentText >= wordCount)
        currentText = 0;
}

/*
* Sets the text that should be displayed.
*
* Parameter text: the text that should be displayed
* Parameter len: the number of characters
*/
int FlashingText::setText(String text) {
    stop();

    if(text.length() < 1)
        return 1;
    
    wordCount = 1;
    for (unsigned int i = 0; i < text.length(); i++) {
        if (text[i] == ';')
            wordCount++;
    }
    
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

    os_timer_setfn(&interruptTimer, updateCurrentText, NULL);
    os_timer_arm(&interruptTimer, displayDuration, true);

    return 0;
}

int FlashingText::setInterval(int interval) {
    // An interval lower than 300 makes no sense
    if (interval < 300)
        return 1;

    displayDuration = interval;
    os_timer_disarm(&interruptTimer);
    os_timer_arm(&interruptTimer, displayDuration, true);

    return 0;
}

void FlashingText::stop() {
    os_timer_disarm(&interruptTimer);
    Matrix::clearAll();
}