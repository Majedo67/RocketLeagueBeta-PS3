#ifndef NOVA_AUDIO_H
#define NOVA_AUDIO_H

#include "menu.h"

static const char* NORMAL_MUSIC_FILE =
    "/app_home/USRDIR/music/rocketleague.mp3";

static const char* SARPBC_MUSIC_FILE =
    "/app_home/USRDIR/music/sarpbc.mp3";

inline const char* getMusicFile(const MenuState& menu) {
    if (menu.music == MUSIC_SARPBC) {
        return SARPBC_MUSIC_FILE;
    }

    return NORMAL_MUSIC_FILE;
}

#endif