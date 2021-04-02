#include "SpotifyMonitor.h"

String lastPlaying = "";

void SpotifyMonitor::checkForNewTrack() {
    if (RunningText::isRunning() || FlashingText::isFlashing()) {
        return;
    }
    String playing = SpotifyClient::getCurrentlyPlaying();
    if (playing != lastPlaying) {
        lastPlaying = playing;
        RunningText::setText(playing);
    }
}