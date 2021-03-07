#include "SchedulerUtils.h"
#include "RunningText.h"
#include "OperateMatrix.h"
#include "WebServer.h"
#include "Configuration.h"
#include "SpotifyClient.h"
#include "WiFiManager.h"

void setup()
{
    Serial.begin(115200);
    Serial.println(" \r\nINIT \r\n");

    WiFiManager::setup();

    Matrix::setup();
    Matrix::clearAll();

    SpotifyClient::setup();

    WebServer::setup();

    SchedulerUtils::enableWebServer();
}

void loop()
{
    SchedulerUtils::executeScheduler();
}