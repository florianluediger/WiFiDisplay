#ifndef OperateMatrix_h
#define OperateMatrix_h

#define MAX_IN_USE 2    // Number of cascaded MAX7219 ICs 

/*
 * Pin configuration for the LED matrices
 */

#define DATA_IN D5
#define LOAD D6
#define CLOCK D7

extern void putByte(byte data);
extern void maxSingle(byte reg, byte col);
extern void maxAll(byte reg, byte col);
extern void maxOne(byte maxNr, byte reg, byte col);
extern void setUp();
extern void clearAll();

extern void inBuffer(int x, int y);
extern void symbolInBuffer(int x, int *arr, int laenge);
extern void setWholeBuffer(int arr[MAX_IN_USE * 8][8]);
extern void setWholeBuffer(int* arr);
extern void drawBuffer();
#endif

