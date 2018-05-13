#ifndef OperateMatrix_h
#define OperateMatrix_h

#include <Arduino.h>
#include "Configuration.h"

namespace Matrix {
    extern void setup();
    extern void clearAll();

    extern void inBuffer(int x, int y);
    extern void symbolInBuffer(int x, int *arr, int laenge);
    extern void setWholeBuffer(int arr[MAX_IN_USE * 8][8]);
    extern void setWholeBuffer(int* arr);
    extern void drawBuffer();
}
#endif

