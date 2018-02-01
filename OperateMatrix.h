#ifndef OperateMatrix_h
#define OperateMatrix_h

extern void putByte(byte data);
extern void maxSingle(byte reg, byte col);
extern void maxAll(byte reg, byte col);
extern void maxOne(byte maxNr, byte reg, byte col);
extern void setUp();
extern void clearAll();

extern void inBuffer(int x, int y);
extern void symbolInBuffer(int x, int *arr, int laenge);
extern void setWholeBuffer(int arr[16][8]);
extern void setWholeBuffer(int* arr);
extern void drawBuffer();
#endif

