#include "FlashingText.h"

int wordCount;
String* flashingBuffer;
int currentText;
int displayDuration = 2000;

os_timer_t interruptTimer;

void setCenteredText(String text) {
    Serial.println("Method called");
    Matrix::symbolInBuffer(5, letters[34 + currentText], 5);
    Matrix::drawBuffer();
}

void updateCurrentText(void *pArg) {
    Serial.println(flashingBuffer[currentText]);
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
void FlashingText::setText(String text, int len) {
    Serial.println(text);
    wordCount = 0;
    /*char *word;
    word = strtok(text, ";");
    do {
        Serial.println(word);
        flashingBuffer[wordCount] = word;
        wordCount++;
        word = strtok(NULL, ";");
    } while (word != NULL);*/

    Serial.println(flashingBuffer[0]);
    Serial.println(flashingBuffer[1]);

    currentText = 0;

    os_timer_setfn(&interruptTimer, updateCurrentText, NULL);
    os_timer_arm(&interruptTimer, displayDuration, true);
}

void FlashingText::setInterval(int interval) {
    displayDuration = interval;
    os_timer_disarm(&interruptTimer);
    os_timer_arm(&interruptTimer, displayDuration, true);
}

void FlashingText::stopFlashing() {
    os_timer_disarm(&interruptTimer);
    Matrix::clearAll();
}