#ifndef GAME_INPUT_H
#define GAME_INPUT_H

#include <io/pad.h>
#include "game.h"

inline void readGameInput(Game& game, const padData& pad,
                          float& throttle, float& steering) {
    steering = clampValue((float(pad.ANA_L_H) - 128.0f) / 127.0f, -1.0f, 1.0f);
    throttle = float(pad.PRE_R2) / 255.0f - float(pad.PRE_L2) / 255.0f;
    throttle = clampValue(throttle, -1.0f, 1.0f);

    game.car.boosting = pad.BTN_R1 && game.car.boost > 0.0f;
    game.car.drifting = pad.BTN_L1;

    if (pad.BTN_CROSS) jumpCar(game);
}

#endif
