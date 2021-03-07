# WiFiDisplay
This is the software for the ESP8266 powering my LED matrices, providing many tools for displaying various data on the display.
Currently implemented are a running text functionality and a flashing text utility.
The data is transferred over WiFi, so the display can be set up in various places with no permanent connection to a computer needed.

You can find the hardware description and the board layout at https://github.com/florianluediger/LedMatrixBoard

## Spotify Connectivity
I have recently added support for Spotify connectivity. This allows to display the currently playing spotify song at any given moment.

My code for accessing the Spotify API is based on the [ThingPulse esp8266-spotify-remote](https://github.com/ThingPulse/esp8266-spotify-remote) repository.