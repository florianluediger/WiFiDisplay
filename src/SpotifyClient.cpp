/**The MIT License (MIT)
 
 Copyright (c) 2018 by ThingPulse Ltd., https://thingpulse.com
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "SpotifyClient.h"

SpotifyAuthenticator client(CLIENT_ID, CLIENT_SECRET, ESPOTIFIER_REDIRECT_URI);
SpotifyAuth auth;

void skipAheadTo(WiFiClientSecure client, String searchString) {
  String l;
  int maxProcessedLines = 500;
  do
  {
    l = client.readStringUntil('\n');
    maxProcessedLines--;
  } while (maxProcessedLines > 0 && !l.isEmpty() && l.indexOf(searchString) == -1);
}

String findSymbol(WiFiClientSecure client, String searchString) {
  String l;
  int index;
  int maxProcessedLines = 500;
  do
  {
    l = client.readStringUntil('\n');
    index = l.indexOf(searchString);
    maxProcessedLines--;
  } while (maxProcessedLines > 0 && !l.isEmpty() && index == -1);

  int endIndex = l.indexOf("\",");
  return l.substring(index + 9, endIndex);
}

void SpotifyClient::setup()
{
  String code = client.startConfigPortal();
  String grantType = "authorization_code";
  client.getToken(&auth, grantType, code);
  Serial.printf("Refresh token: %s\nAccess Token: %s\n", auth.refreshToken.c_str(), auth.accessToken.c_str());
}

String SpotifyClient::getCurrentlyPlaying()
{
  WiFiClientSecure client = WiFiClientSecure();
  client.setInsecure();

  String host = "api.spotify.com";
  const int port = 443;
  String url = "/v1/me/player/currently-playing";
  if (!client.connect(host.c_str(), port))
  {
    Serial.println("ERROR - connection failed");
    return "ERROR - connection failed";
  }

  String request = "GET " + url + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "Authorization: Bearer " + auth.accessToken + "\r\n" +
                   "Content-Length: 0\r\n" +
                   "Connection: close\r\n\r\n";
  // This will send the request to the server
  client.print(request);

  int retryCounter = 0;
  while (!client.available())
  {
    retryCounter++;
    if (retryCounter > 10)
    {
      Serial.println("ERROR - request timed out");
      return "ERROR - request timed out";
    }
    delay(50);
  }

  client.setNoDelay(false);

  // Skip HTTP headers
  if (!client.find(endOfHeaders))
  {
    Serial.println(F("Invalid response"));
    client.stop();
    return "ERROR - invalid response";
  }

  // JSON parsing is not feasible for this request because the chip memory is not large enough for the response

  String artist = "";
  String title = "";

  skipAheadTo(client, "item");
  skipAheadTo(client, "artists");
  artist = findSymbol(client, "name");
  skipAheadTo(client, "duration_ms");
  title = findSymbol(client, "name");

  client.stop();

  return artist + " - " + title;
}

uint16_t SpotifyClient::playerCommand(String method, String command)
{
  WiFiClientSecure client = WiFiClientSecure();
  client.setInsecure();

  String host = "api.spotify.com";
  const int port = 443;
  String url = "/v1/me/player/" + command;
  if (!client.connect(host.c_str(), port))
  {
    Serial.println("connection failed");
    return 0;
  }

  String request = method + " " + url + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "Authorization: Bearer " + auth.accessToken + "\r\n" +
                   "Content-Length: 0\r\n" +
                   "Connection: close\r\n\r\n";
  // This will send the request to the server
  client.print(request);

  int retryCounter = 0;
  while (!client.available())
  {
    retryCounter++;
    if (retryCounter > 10)
    {
      return 0;
    }
    delay(50);
  }

  int size = 0;
  client.setNoDelay(false);
  uint16_t httpCode = 0;
  while (client.connected() || client.available())
  {
    while ((size = client.available()) > 0)
    {
      String line = client.readStringUntil('\r');
      Serial.println(line);
      if (line.startsWith("HTTP/1."))
      {
        httpCode = line.substring(9, line.indexOf(' ', 9)).toInt();
        Serial.printf("HTTP Code: %d\n", httpCode);
      }
    }
  }

  return httpCode;
}
