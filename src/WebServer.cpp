#include "WebServer.h"
#include "Configuration.h"
#include <ESP8266WebServer.h>

ESP8266WebServer webServer(80);

/**
 * Handles requests for displaying a running text or changing the running speed.
 */
void handleRunningText() {
    String text = webServer.arg("text");
    String interval = webServer.arg("interval");

    // An empty request is not valid
    if (interval.length() <= 0 && text.length() <= 0) {
        webServer.send(400, "text/plain", "Your request did not contain any valid data.");
        return;
    }

    if (interval.length() > 0) {
        int success = RunningText::setInterval(atoi(interval.c_str()));
        if (success != 0) {
            webServer.send(400, "text/plain", "Interval must not be smaller than 50.");
            return;
        }
    }

    if (text.length() > 0)
        RunningText::setText(text);

    webServer.send(200);
}

/**
 * Handles request for stopping the running text.
 */
void handleStopRunningText() {
    RunningText::stop();
    webServer.send(200);
}

/**
 * Handles requests for checking, if the running text is currently being displayed.
 */
void handleRunningTextIsRunning() {
    webServer.send(200, "text/plain", (String)RunningText::isRunning());
}

/**
 * Handles requests for displaying a flashing text or changing the flashing duration.
 */
void handleFlashingText() {
    String text = webServer.arg("text");
    String interval = webServer.arg("interval");
    String iterations = webServer.arg("iterations");

    // An empty request is not valid
    if (interval.length() <= 0 && text.length() <= 0) {
        webServer.send(400, "text/plain", "Your request did not contain any valid data.");
        return;
    }

    if (interval.length() > 0) {
        int success = FlashingText::setInterval(atoi(interval.c_str()));
        if (success != 0) {
            webServer.send(400, "text/plain", "The interval is too low to make sense.");
            return;
        }
    }

    if (text.length() > 0) {
        int success = FlashingText::setText(text, atoi(iterations.c_str()));
        if (success != 0) {
            webServer.send(400, "text/plain", "Your string is not valid, consider splitting words into multiple sections.");
        }
    }
    
    webServer.send(200);
}

/**
 * Handles requests for stopping the flashing text.
 */
void handleStopFlashingText() {
    FlashingText::stop();
    webServer.send(200);
}

/**
 * Handles requests for checking, if the flashing text is currently being displayed.
 */
void handleFlashingTextIsFlashing() {
    webServer.send(200, "text/plain", (String)FlashingText::isFlashing());
}

/**
 * Handles requests for checking, if any text is currently being displayed.
 */
void handleAnyDisplaying() {
    bool isDisplaying = RunningText::isRunning() || FlashingText::isFlashing();
    webServer.send(200, "text/plain", (String)isDisplaying);
}

/**
 * Handles requests for stopping anything that is currently being displayed.
 */
void handleStopAny() {
    RunningText::stop();
    FlashingText::stop();
    webServer.send(200);
}

void handleSpotifyCurrent() {
    String playing = SpotifyClient::getCurrentlyPlaying();
    RunningText::setText(playing);
    webServer.send(200);
}

/**
 * Starts the web server to be able to handle different requests.
 */
void WebServer::setup() {
    webServer.on("/text/running", handleRunningText);
    webServer.on("/text/running/stop", handleStopRunningText);
    webServer.on("/text/running/isrunning", handleRunningTextIsRunning);
    webServer.on("/text/flashing", handleFlashingText);
    webServer.on("/text/flashing/stop", handleStopFlashingText);
    webServer.on("/text/flashing/isflashing", handleFlashingTextIsFlashing);
    webServer.on("/text/any/displaying", handleAnyDisplaying);
    webServer.on("/text/any/stop", handleStopAny);
    webServer.on("/spotify/current", handleSpotifyCurrent);
    webServer.on("/spotify/enable", SchedulerUtils::enableSpotifyMonitor);
    webServer.on("/spotify/disable", SchedulerUtils::disableSpotifyMonitor);
    webServer.onNotFound([]() { webServer.send(404); });
    webServer.begin();

    Serial.println("HTTP server has started.");
}

/**
 * Checks, if any new requests have occurred since the last check.
 */
void WebServer::checkForRequest() {
    webServer.handleClient();
}