#include "SpotifyMonitor.h"

String lastPlaying = "";

void SpotifyMonitor::checkForNewTrack() {
    String playing = SpotifyClient::getCurrentlyPlaying();
    if (playing != lastPlaying) {
        lastPlaying = playing;
        RunningText::setText(playing);
    }
}