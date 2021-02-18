#ifndef Scheduler_h
#define Scheduler_h

#include "RunningText.h"
#include "WebServer.h"

namespace SchedulerUtils {
    extern void executeScheduler();
    extern void enableWebServer();
    extern void enableRunningText();
    extern void disableRunningText();
    extern void enableFlashingText();
    extern void disableFlashingText();
}
#endif
