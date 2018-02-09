#include "RunningText.h"
#include "Configuration.h"
#include "OperateMatrix.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/*
 * Network Configuration
 */
IPAddress ip(IP);
IPAddress gateway(GATEWAY);
IPAddress subnet(SUBNET);

ESP8266WebServer server(80);

RunningText rt;

void handleRoot() {
  String text = server.arg("text");
  server.send(200);
  char* textInput = (char*)malloc((text.length() + 1) * sizeof(char));
  text.toCharArray(textInput, text.length() + 1);

  rt.setText(textInput, strlen(textInput));
}

void setup() {
  setUp();
  clearAll();
  
  Serial.begin(115200);
  Serial.println(" \r\nINIT \r\n") ;
  
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
  
  server.on("/",handleRoot);
  server.onNotFound([]() { server.send(404); });
  server.begin();
  
  Serial.println("HTTP server has started!");
}

void loop() {
  server.handleClient();
  rt.updatePosition();
  delay(100);
}

