
#ifndef GAME_INPUT_H
#define GAME_INPUT_H

#include <io/pad.h>
#include "game.cpp"

inline void readGameInput(
    Game& game,
    const padData& pad,
    float& throttle,
    float& steering
) {
    // Left stick: steering
    steering = (float(pad.ANA_L_H) - 128.0f) / 127.0f;

    // R2 accelerates, L2 reverses
    throttle = float(pad.PRE_R2) / 255.0f
             - float(pad.PRE_L2) / 255.0f;

    // X jumps
    if (pad.BTN_CROSS) {
        jumpCar(game);
    }
}

#endif
