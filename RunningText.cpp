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
 * Sets the text that should be displayed.
 *
 * Parameter text: the text that should be displayed
 * Parameter len: the number of characters
 */
void RunningText::setText(char *text, int len) {
  bufferWidth = (len * 6) + (MAX_IN_USE * 16);
  runningBuffer = (int*)malloc(bufferWidth * sizeof(int));

  int pos = 0;
  symbolInRunningBuffer(pos, empty, MAX_IN_USE * 8);
  pos += (MAX_IN_USE * 8);

  for (int i = 0; i < len; i++) {
    symbolInRunningBuffer(pos, letters[text[i] - 32], 5);
	pos += 5;
	runningBuffer[pos] = 0;
	pos++;
  }

  symbolInRunningBuffer(pos, empty, MAX_IN_USE * 8);

  position = 0;
}


/*
 * Shifts the whole text one pixel to the left.
 * Stops when the whole text has been displayed
 */
void RunningText::updatePosition() {
  if (position < (bufferWidth - (MAX_IN_USE * 8))) {
    setWholeBuffer(runningBuffer + position);
    drawBuffer();
    position++;
  }
}
