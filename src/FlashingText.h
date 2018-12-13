#ifndef FlashingText_h
#define FlashingText_h

#include "OperateMatrix.h"
#include "MatrixConstants.h"
#include "user_interface.h"

namespace FlashingText {
    extern int setText(String text);
    extern int setInterval(int interval);
    extern void stop();
    extern bool isFlashing();
}
#endif

