#include "WiFiManager.h"
#include "Configuration.h"
#include <ESP8266WiFi.h>

/**
 * Network Configuration
 */
IPAddress ip(ESP_IP);
IPAddress gateway(ESP_GATEWAY);
IPAddress subnet(ESP_SUBNET);
IPAddress dnsserver(DNS_SERVER);

/**
 * Sets the ESP up in AP mode and creates a WiFi network.
 */
void setupAP() {
    WiFi.mode(WIFI_AP);

    Serial.println(WiFi.softAPConfig(ip, gateway, subnet) ? "Config ready" : "Config failed");

    Serial.println(WiFi.softAP(NETWORK, PASS) ? "WiFi ready" : "WiFi failed");

    Serial.println("WiFi AP address:");
    Serial.println(WiFi.softAPIP());
}

/**
 * Sets the ESP up in station mode and connects to a WiFi network.
 */
void setupStation() {
    WiFi.hostname("esp8266");
    WiFi.begin(NETWORK, PASS);
    WiFi.config(ip, gateway, subnet, dnsserver);
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

void WiFiManager::setup() {
    if (NETWORK_MODE == 0)
        setupStation();
    else if (NETWORK_MODE == 1)
        setupAP();
    else
        Serial.println("Unknown network mode configuration");
}