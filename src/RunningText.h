#ifndef RunningText_h
#define RunningText_h

#include "SchedulerUtils.h"
#include "OperateMatrix.h"
#include "MatrixConstants.h"
#include "user_interface.h"
#include "FlashingText.h"

namespace RunningText {
    extern void setText(String text);
    extern int setInterval(int interval);
    extern void stop();
    extern bool isRunning();
    extern void updatePosition();
}
#endif

