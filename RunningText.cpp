#include "RunningText.h"

/*
 * Puts a symbol in the running buffer so it can be displayed later.
 *
 * Parameter x: how far to the right the symbol should be placed in pixels
 * Parameter arr: array containing the symbol, see MatrixConstants.h file for examples
 * Parameter len: width of the symbol in pixels
 */
void RunningText::symbolInRunningBuffer(int x, int* arr, int len) {
  for (int i = 0; i<len; i++) {
    runningBuffer[x+i] = *(arr+i);
  }
}

/*
 * Displays a running text on the display
 *
 * Parameter text: the text that should be displayed
 * Parameter len: the number of characters
 * Parameter del: delay between steps of the running text in milliseconds
 */
void RunningText::runningText(char *text, int len, int del) {
  int xWidth = len * 6;
  runningBuffer = (int*)malloc((xWidth + (MAX_IN_USE * 16)) * sizeof(int));
  int pos = 0;
  symbolInRunningBuffer(pos, empty, MAX_IN_USE * 8);
  pos += (MAX_IN_USE * 8);
  for (int i = 0; i < len; i++) {
    symbolInRunningBuffer(pos, letters[text[i]-32], 5);
    pos += 5;
    runningBuffer[pos] = 0;
    pos++;
  }
  symbolInRunningBuffer(pos, empty, MAX_IN_USE * 8);

  for (int x = 0; x < xWidth + (MAX_IN_USE * 8); x++) {
    setWholeBuffer(runningBuffer + x);
    drawBuffer();
    delay(del);
  }
}

