#include "RunningText.h"
#include "OperateMatrix.h"
#include "WebServer.h"


void setup() {
    Serial.begin(115200);
    Serial.println(" \r\nINIT \r\n");

    Matrix::setup();
    Matrix::clearAll();

    WebServer::setup();
}

void loop() {
    WebServer::checkForRequest();
    RunningText::updatePosition();
    delay(50);
}

