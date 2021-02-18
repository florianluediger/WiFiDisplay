#include "SchedulerUtils.h"
#include <TaskScheduler.h>

Scheduler* scheduler = new Scheduler();

Task* webserver = new Task(0, TASK_FOREVER, &WebServer::checkForRequest, scheduler);
Task* runningtext = new Task(50, TASK_FOREVER, &RunningText::updatePosition, scheduler);
Task* flashingtext = new Task(500, TASK_FOREVER, &FlashingText::updateCurrentText, scheduler);

void SchedulerUtils::executeScheduler() {
    scheduler->execute();
}

void SchedulerUtils::enableWebServer() {
    webserver->enable();
}

void SchedulerUtils::enableRunningText() {
    runningtext->enable();
}

void SchedulerUtils::disableRunningText() {
    runningtext->disable();
}

void SchedulerUtils::enableFlashingText() {
    flashingtext->enable();
}

void SchedulerUtils::disableFlashingText() {
    flashingtext->disable();
}