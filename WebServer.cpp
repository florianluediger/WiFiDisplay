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
    webServer.send(200);
    char* textInput = (char*)malloc((text.length() + 1) * sizeof(char));
    text.toCharArray(textInput, text.length() + 1);

    RunningText::setText(textInput, strlen(textInput));
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