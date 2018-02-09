#ifndef RunningText_h
#define RunningText_h

#include "OperateMatrix.h"
#include "MatrixConstants.h"

class RunningText {
  private:
    int* runningBuffer;
	int bufferWidth;
	int position;

    void symbolInRunningBuffer(int x, int* arr, int len);

  public:
    void setText(char *text, int len);
	void updatePosition();
};

#endif

