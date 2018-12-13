#ifndef FlashingText_h
#define FlashingText_h

#include "OperateMatrix.h"
#include "MatrixConstants.h"
#include "user_interface.h"

namespace FlashingText {
    extern void setText(String text, int len);
    extern void setInterval(int interval);
    extern void stop();
}
#endif

