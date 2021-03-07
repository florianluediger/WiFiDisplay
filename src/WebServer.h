#ifndef WebServer_h
#define WebServer_h

#include "RunningText.h"
#include "FlashingText.h"
#include "SpotifyClient.h"

namespace WebServer {
    extern void setup();
    extern void checkForRequest();
}
#endif