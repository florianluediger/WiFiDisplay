#include "OperateMatrix.h"
#include "MatrixConstants.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/*
 * Network Configuration
 */
const char* ssid = "Your SSID here";    // Your WiFi SSID
const char* pass = "Your password here";// Your WiFi password
IPAddress ip(10,0,0,70);                // The desired IP of your WiFiDisplay
IPAddress gateway(10,0,0,1);            // The IP of your router
IPAddress subnet(255,255,255,0);        // Your configured subnet mask

char* timeDisplay = "9999";
int* runningBuffer;

ESP8266WebServer server(80);

/*
 * Draws the time contained in the timeDisplay variable onto the display.
 */
void drawTime() {
  symbolInBuffer(0,numbers[timeDisplay[0]-48], 3);
  symbolInBuffer(4,numbers[timeDisplay[1]-48], 3);
  symbolInBuffer(9,numbers[timeDisplay[2]-48], 3);
  symbolInBuffer(13,numbers[timeDisplay[3]-48], 3);
  drawBuffer();
}

/*
 * Puts a symbol in the running buffer so it can be displayed later.
 * 
 * Parameter x: how far to the right the symbol should be placed in pixels
 * Parameter arr: array containing the symbol, see MatrixConstants.h file for examples
 * Parameter len: width of the symbol in pixels
 */
void symbolInRunningBuffer(int x, int* arr, int len) {
  for (int i = 0; i<len; i++) {
    runningBuffer[x+i] = *(arr+i);
  }
}

/*
 * Displays a running text on the display
 * 
 * Parameter text: the text that should be displayed
 * Parameter len: the number of characters
 * Parameter del: delay between steps of the running text in milliseconds
 */
void runningText(char *text, int len, int del) {
  int xWidth = len * 6;
  runningBuffer = (int*)malloc((xWidth + 32) * sizeof(int));
  int pos = 0;
  symbolInRunningBuffer(pos, empty, 16);
  pos += 16;
  for (int i = 0; i < len; i++) {
    symbolInRunningBuffer(pos, letters[text[i]-32], 5);
    pos += 5;
    runningBuffer[pos] = 0;
    pos++;
  }
  symbolInRunningBuffer(pos, empty, 16);

  for (int x = 0; x < xWidth + 16; x++) {
    setWholeBuffer(runningBuffer + x);
    drawBuffer();
    delay(del);
  }
}

void handleRoot() {
  String text = server.arg("text");
  char* textInput = (char*)malloc((text.length() + 1) * sizeof(char));
  text.toCharArray(textInput, text.length() + 1);
  runningText(textInput, strlen(textInput), 100);
}

void setup() {
  setUp();
  clearAll();
  
  Serial.begin(115200);
  Serial.println(" \r\nINIT \r\n") ;
  
  WiFi.begin(ssid, pass);
  WiFi.config(ip, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  } 
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  server.on("/",handleRoot);
  server.begin();
  
  Serial.println("HTTP server has started!");
}

void loop() {
  server.handleClient();
  delay(100);
}

