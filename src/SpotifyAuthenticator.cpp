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

#include "SpotifyAuthenticator.h"

SpotifyAuthenticator::SpotifyAuthenticator(String clientId, String clientSecret, String redirectUri)
{
  this->clientId = clientId;
  this->clientSecret = clientSecret;
  this->redirectUri = redirectUri;
}

void SpotifyAuthenticator::getToken(SpotifyAuth *auth, String grantType, String code)
{
  WiFiClientSecure client;
  //https://accounts.spotify.com/api/token
  const char *host = "accounts.spotify.com";
  const int port = 443;
  String url = "/api/token";
  client.setInsecure();
  if (!client.connect(host, port))
  {
    Serial.println("connection failed");
    return;
  }

  String codeParam = "code";
  if (grantType == "refresh_token")
  {
    codeParam = "refresh_token";
  }
  String authorizationRaw = clientId + ":" + clientSecret;
  String authorization = base64::encode(authorizationRaw, false);
  // This will send the request to the server
  String content = "grant_type=" + grantType + "&" + codeParam + "=" + code + "&redirect_uri=" + redirectUri;
  String request = String("POST ") + url + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "Authorization: Basic " + authorization + "\r\n" +
                   "Content-Length: " + String(content.length()) + "\r\n" +
                   "Content-Type: application/x-www-form-urlencoded\r\n" +
                   "Connection: close\r\n\r\n" +
                   content;
  client.print(request);

  int retryCounter = 0;
  while (!client.available())
  {
    retryCounter++;
    if (retryCounter > 10)
    {
      return;
    }
    delay(50);
  }

  client.setNoDelay(false);

  // Skip HTTP headers
  if (!client.find(endOfHeaders))
  {
    Serial.println(F("Invalid response"));
    client.stop();
    return;
  }

  DynamicJsonDocument doc(768);

  // Parse JSON object
  DeserializationError error = deserializeJson(doc, client);
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    client.stop();
    return;
  }

  auth->accessToken = doc["access_token"].as<char *>();
  auth->refreshToken = doc["refresh_token"].as<char *>();

  client.stop();
}

String SpotifyAuthenticator::startConfigPortal()
{
  String oneWayCode = "";

  server.on("/", [this]() {
    Serial.println(this->clientId);
    Serial.println(this->redirectUri);
    server.sendHeader("Location", String("https://accounts.spotify.com/authorize/?client_id=" + this->clientId + "&response_type=code&redirect_uri=" + this->redirectUri + "&scope=user-read-private%20user-read-currently-playing%20user-read-playback-state%20user-modify-playback-state"), true);
    server.send(302, "text/plain", "");
  });

  server.on("/callback/", [this, &oneWayCode]() {
    if (!server.hasArg("code"))
    {
      server.send(500, "text/plain", "BAD ARGS");
      return;
    }

    oneWayCode = server.arg("code");
    Serial.printf("Code: %s\n", oneWayCode.c_str());

    String message = "<html><head></head><body>Succesfully authentiated This device with Spotify. Restart your device now</body></html>";

    server.send(200, "text/html", message);
  });

  server.begin();

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("WiFi connected!");
  }
  else
  {
    Serial.println("WiFi not connected!");
  }

  Serial.println("HTTP server started");

  while (oneWayCode == "")
  {
    server.handleClient();
    yield();
  }
  server.stop();
  return oneWayCode;
}