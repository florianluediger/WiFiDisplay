#ifndef RunningText_h
#define RunningText_h

#include "OperateMatrix.h"
#include "MatrixConstants.h"

class RunningText {
  private:
    int* runningBuffer;

    void symbolInRunningBuffer(int x, int* arr, int len);

  public:
    void runningText(char *text, int len, int del);
};

#endif

