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

void handleText() {
    String text = webServer.arg("text");
    String intervalParam = webServer.arg("interval");
    String mode = webServer.arg("mode");

    if (mode.length() == 0 || mode == "running") {
        if (text.length() > 0) {
            RunningText::setText(text, text.length());
        }
    }
    else if (mode == "flashing") {
        if (text.length() > 0) {
            FlashingText::setText(text, text.length());
        }
    }
    else {
        webServer.send(400, "text/plain", "Mode must be either running or flashing!");
    }    

    if (intervalParam.length() > 0) {
        int interval = atoi(intervalParam.c_str());
        if (interval < 50) {
            webServer.send(400, "text/plain", "Interval must not be smaller than 50!");
            return;
        }
        RunningText::setInterval(interval);
    }

    webServer.send(200);
}

void WebServer::setup() {
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

    webServer.on("/text", handleText);
    webServer.onNotFound([]() { webServer.send(404); });
    webServer.begin();

    Serial.println("HTTP server has started!");
}

void WebServer::checkForRequest() {
    webServer.handleClient();
}