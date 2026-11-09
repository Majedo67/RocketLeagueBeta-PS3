#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <io/pad.h>
#include "menu.h"
#include "game_input.h"

inline bool initializeController() {
    return ioPadInit(1) == 0;
}

inline void shutdownController() {
    ioPadEnd();
}

inline void updateController(
    MenuState& menu,
    Game& game,
    bool& confirm,
    float& throttle,
    float& steering
) {
    padData pad{};

    confirm = false;
    throttle = 0.0f;
    steering = 0.0f;

    if (ioPadGetData(0, &pad) != 0 || pad.len == 0)
        return;

    if (!menu.visible) {
        readGameInput(game, pad, throttle, steering);
        return;
    }

    if (pad.BTN_UP) {
        if (menu.waitingForStart) {
            pressAnyButtonToStart(menu);
        } else {
            registerSecretInput(menu, SECRET_UP);
            moveMenuSelection(menu, -1);
        }
    }

    if (pad.BTN_DOWN) {
        registerSecretInput(menu, SECRET_DOWN);
        moveMenuSelection(menu, 1);
    }

    if (pad.BTN_LEFT)
        registerSecretInput(menu, SECRET_LEFT);

    if (pad.BTN_RIGHT)
        registerSecretInput(menu, SECRET_RIGHT);

    if (pad.BTN_CROSS) {
        if (menu.waitingForStart) {
            pressAnyButtonToStart(menu);
        } else {
            registerSecretInput(menu, SECRET_X);
            confirm = true;
        }
    }

    if (pad.BTN_CIRCLE)
        registerSecretInput(menu, SECRET_O);

    if (pad.BTN_START) {
        if (menu.waitingForStart)
            pressAnyButtonToStart(menu);

        registerSecretInput(menu, SECRET_START);
    }
}

#endif