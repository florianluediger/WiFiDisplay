#include "WebServer.h"
#include "Configuration.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/*
* Network Configuration
*/
IPAddress ip(IP);
IPAddress gateway(GATEWAY);
IPAddress subnet(SUBNET);

ESP8266WebServer webServer(80);

void handleRunningText() {
    String text = webServer.arg("text");
    String intervalParam = webServer.arg("interval");
    
    int interval = 0;
    if (intervalParam.length() > 0)
        interval = atoi(intervalParam.c_str());

    // An empty request is not valid
    if (interval == 0 && text.length() <= 0) {
        webServer.send(400, "text/plain", "Your request did not contain any valid data!");
        return;
    }

    // An interval lower than 50 causes problems
    if (interval > 0 && interval < 50) {
        webServer.send(400, "text/plain", "Interval must not be smaller than 50!");
        return;
    }

    if (interval > 50)
        RunningText::setInterval(interval);

    if (text.length() > 0) {
        RunningText::setText(text, text.length());
    }

    webServer.send(200);
}

void handleFlashingText() {
    String text = webServer.arg("text");

    if (text.length() <= 0) {
        webServer.send(400, "text/plain", "Your request did not contain any valid data!");
        return;
    }

    FlashingText::setText(text, text.length());
    
    webServer.send(200);
}

void setupAP() {
    WiFi.mode(WIFI_AP);

    Serial.println(WiFi.softAPConfig(ip, gateway, subnet) ? "Config ready" : "Config failed");

    Serial.println(WiFi.softAP(NETWORK, PASS) ? "WiFi ready" : "WiFi failed");

    Serial.println("WiFi AP address:");
    Serial.println(WiFi.softAPIP());
}

void setupStation() {
    WiFi.begin(NETWORK, PASS);
    WiFi.config(ip, gateway, subnet);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(NETWORK);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void WebServer::setup() {
    if (NETWORK_MODE == 0)
        setupStation();
    else if (NETWORK_MODE == 1)
        setupAP();
    else
        Serial.println("Unknown network mode configuration");

    webServer.on("/text/running", handleRunningText);
    webServer.on("/text/flashing", handleFlashingText);
    webServer.onNotFound([]() { webServer.send(404); });
    webServer.begin();

    Serial.println("HTTP server has started!");
}

void WebServer::checkForRequest() {
    webServer.handleClient();
}