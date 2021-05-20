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

void skipAheadTo(WiFiClientSecure clientSecure, String searchString) {
  String l;
  int maxProcessedLines = 500;
  do
  {
    l = clientSecure.readStringUntil('\n');
    maxProcessedLines--;
  } while (maxProcessedLines > 0 && !l.isEmpty() && l.indexOf(searchString) == -1);
}

String findSymbol(WiFiClientSecure clientSecure, String searchString) {
  String l;
  int index;
  int maxProcessedLines = 500;
  do
  {
    l = clientSecure.readStringUntil('\n');
    index = l.indexOf(searchString);
    maxProcessedLines--;
  } while (maxProcessedLines > 0 && !l.isEmpty() && index == -1);

  int endIndex = l.indexOf("\",");
  return l.substring(index + 9, endIndex);
}

uint16_t executeRequest(WiFiClientSecure clientSecure, String request) {
  clientSecure.print(request);

  int retryCounter = 0;
  while (!clientSecure.available())
  {
    retryCounter++;
    if (retryCounter > 10)
    {
      Serial.println("Request was cancelled because of too many retries");
      return 408;
    }
    delay(50);
  }

  clientSecure.setNoDelay(false);

  // Read HTTP Code from response
  if (!clientSecure.find("HTTP/1.")) {
      Serial.println("HTTP code could not be found");
      return 500;
  }

  String line = clientSecure.readStringUntil('\r');

  uint16_t httpCode = line.substring(2, line.indexOf(' ', 2)).toInt();

  if (httpCode == 400) {
    return httpCode;
  }

  // Skip HTTP headers
  clientSecure.find(endOfHeaders);

  return httpCode;
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
  WiFiClientSecure clientSecure = WiFiClientSecure();
  clientSecure.setInsecure();

  String host = "api.spotify.com";
  const int port = 443;
  String url = "/v1/me/player/currently-playing";
  if (!clientSecure.connect(host.c_str(), port))
  {
    Serial.println("ERROR - connection failed");
    clientSecure.stop();
    return "ERROR - connection failed";
  }

  String request = "GET " + url + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "Authorization: Bearer " + auth.accessToken + "\r\n" +
                   "Content-Length: 0\r\n" +
                   "Connection: close\r\n\r\n";

  uint16_t httpCode = executeRequest(clientSecure, request);

  // If token expired, obtain refresh token and execute method again
  if (httpCode == 401) {
    Serial.println("Refresh Token will be acquired");
    String oldToken = auth.accessToken;
    clientSecure.stop();
    client.getToken(&auth, "refresh_token", auth.refreshToken);
    if (!auth.accessToken.equals(oldToken)) {
      Serial.println("New refresh token successfully acquired");
      return getCurrentlyPlaying();
    } else {
      Serial.println("New refresh token could not be acquired");
      return "";
    }
  }

  if (httpCode == 400) {
    clientSecure.stop();
    return "";
  }

  // JSON parsing is not feasible for this request because the chip memory is not large enough for the response
  String artist = "";
  String title = "";

  skipAheadTo(clientSecure, "item");
  skipAheadTo(clientSecure, "artists");
  artist = findSymbol(clientSecure, "name");
  skipAheadTo(clientSecure, "duration_ms");
  title = findSymbol(clientSecure, "name");

  clientSecure.stop();

  return artist + " - " + title;
}

uint16_t SpotifyClient::playerCommand(String method, String command)
{
  WiFiClientSecure clientSecure = WiFiClientSecure();
  clientSecure.setInsecure();

  String host = "api.spotify.com";
  const int port = 443;
  String url = "/v1/me/player/" + command;
  if (!clientSecure.connect(host.c_str(), port))
  {
    Serial.println("connection failed");
    clientSecure.stop();
    return 0;
  }

  String request = method + " " + url + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "Authorization: Bearer " + auth.accessToken + "\r\n" +
                   "Content-Length: 0\r\n" +
                   "Connection: close\r\n\r\n";
  // This will send the request to the server
  uint16_t httpCode = executeRequest(clientSecure, request);

  // If token expired, obtain refresh token and execute method again
  if (httpCode == 401) {
    String oldToken = auth.accessToken;
    clientSecure.stop();
    client.getToken(&auth, "refresh_token", auth.refreshToken);
    if (!auth.accessToken.equals(oldToken)) {
      return playerCommand(method, command);
    }
  }

  clientSecure.stop();
  return httpCode;
}
