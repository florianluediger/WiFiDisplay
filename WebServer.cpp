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
    int interval = atoi(webServer.arg("interval").c_str());
    webServer.send(200);

    if (text.length() > 0) {
        const char* textInput = text.c_str();
        RunningText::setText(textInput, strlen(textInput));
    }
    if (interval > 0) {
        RunningText::setInterval(interval);
    }
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