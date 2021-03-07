#ifndef Configuration_h
#define Configuration_h

/**
 * Number of cascaded MAX7219 ICs
 */
#define MAX_IN_USE 8

/**
 * Pin configuration for the LED matrices
 */
#define DATA_IN D5
#define LOAD D6
#define CLOCK D7

/**
 * Network configuration
 */
#define NETWORK_MODE 0   // Possible values: 0 for Station mode and 1 for AP mode
#define NETWORK "Your SSID here"  // Your WiFi SSID
#define PASS "Your password here" // Your WiFi password
#define ESP_IP 10,0,0,70              // The desired IP of your WiFiDisplay
#define ESP_GATEWAY 10,0,0,1          // The IP of your router
#define ESP_SUBNET 255,255,255,0      // Your configured subnet mask
#define DNS_SERVER 8,8,8,8  // Your configured dns server (google for example)

/**
 * Spotify settings
 */ 
#define CLIENT_ID "Your Client ID here"
#define CLIENT_SECRET "Your Client Secret here"
// These next two lines have to match. A client clientSpotifierHostName makes the
// node listen on http://esp8266.local. Spotify needs an exact match of the redirect url
#define ESPOTIFIER_NODE_NAME "10.0.0.70"
#define ESPOTIFIER_REDIRECT_URI "http%3A%2F%2F10.0.0.70%2Fcallback%2F"

#endif
