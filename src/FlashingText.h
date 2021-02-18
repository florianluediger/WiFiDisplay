#ifndef FlashingText_h
#define FlashingText_h

#include "SchedulerUtils.h"
#include "OperateMatrix.h"
#include "MatrixConstants.h"
#include "user_interface.h"
#include "RunningText.h"

namespace FlashingText {
    extern int setText(String text, int iterations);
    extern int setInterval(int interval);
    extern void stop();
    extern bool isFlashing();
    extern void updateCurrentText();
}
#endif

