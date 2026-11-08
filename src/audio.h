#ifndef NOVA_AUDIO_H
#define NOVA_AUDIO_H

#include "menu.h"
#include <soundlib/audioplayer.h>

static char NORMAL_MUSIC_FILE[] =
    "/app_home/USRDIR/music/rocketleague.mp3";

static char SARPBC_MUSIC_FILE[] =
    "/app_home/USRDIR/music/sarpbc.mp3";

struct AudioState {
    MusicTrack currentTrack;
    bool playing;
};

inline void initializeAudio(AudioState& audio,
                            const MenuState& menu) {
    audio.playing = false;
    audio.currentTrack = menu.music;

    char* file = (menu.music == MUSIC_SARPBC)
        ? SARPBC_MUSIC_FILE
        : NORMAL_MUSIC_FILE;

    if (PlayAudio(file, 0, AUDIO_INFINITE_TIME) == 0) {
        audio.playing = true;
    }
}

inline void updateAudio(AudioState& audio,
                        const MenuState& menu) {
    if (audio.currentTrack == menu.music) {
        return;
    }

    StopAudio();

    audio.currentTrack = menu.music;

    char* file = (menu.music == MUSIC_SARPBC)
        ? SARPBC_MUSIC_FILE
        : NORMAL_MUSIC_FILE;

    audio.playing =
        (PlayAudio(file, 0, AUDIO_INFINITE_TIME) == 0);
}

inline void shutdownAudio(AudioState& audio) {
    if (audio.playing) {
        StopAudio();
        audio.playing = false;
    }
}

#endif