#ifndef Configuration_h
#define Configuration_h

/*
 * Number of cascaded MAX7219 ICs
 */
#define MAX_IN_USE 4

/*
 * Pin configuration for the LED matrices
 */
#define DATA_IN D5
#define LOAD D6
#define CLOCK D7

/*
 * Network configuration
 */
#define NETWORK "Your SSID here"  // Your WiFi SSID
#define PASS "Your password here" // Your WiFi password
#define IP 10,0,0,70              // The desired IP of your WiFiDisplay
#define GATEWAY 10,0,0,1          // The IP of your router
#define SUBNET 255,255,255,0      // Your configured subnet mask

#endif
