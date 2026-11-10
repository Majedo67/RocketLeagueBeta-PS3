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

inline void updateController(MenuState& menu, Game& game, bool& confirm,
                             float& throttle, float& steering) {
    static padData previousPad = {};
    padData pad = {};

    confirm = false;
    throttle = 0.0f;
    steering = 0.0f;

    if (ioPadGetData(0, &pad) != 0 || pad.len == 0) return;

    const bool upPressed = pad.BTN_UP && !previousPad.BTN_UP;
    const bool downPressed = pad.BTN_DOWN && !previousPad.BTN_DOWN;
    const bool leftPressed = pad.BTN_LEFT && !previousPad.BTN_LEFT;
    const bool rightPressed = pad.BTN_RIGHT && !previousPad.BTN_RIGHT;
    const bool crossPressed = pad.BTN_CROSS && !previousPad.BTN_CROSS;
    const bool circlePressed = pad.BTN_CIRCLE && !previousPad.BTN_CIRCLE;
    const bool startPressed = pad.BTN_START && !previousPad.BTN_START;

    if (!menu.visible) {
        readGameInput(game, pad, throttle, steering);
        previousPad = pad;
        return;
    }

    if (upPressed) {
        if (menu.waitingForStart) pressAnyButtonToStart(menu);
        else {
            registerSecretInput(menu, SECRET_UP);
            moveMenuSelection(menu, -1);
        }
    }
    if (downPressed) {
        if (menu.waitingForStart) pressAnyButtonToStart(menu);
        else {
            registerSecretInput(menu, SECRET_DOWN);
            moveMenuSelection(menu, 1);
        }
    }
    if (leftPressed) registerSecretInput(menu, SECRET_LEFT);
    if (rightPressed) registerSecretInput(menu, SECRET_RIGHT);

    if (crossPressed) {
        if (menu.waitingForStart) pressAnyButtonToStart(menu);
        else {
            registerSecretInput(menu, SECRET_X);
            confirm = true;
        }
    }
    if (circlePressed) registerSecretInput(menu, SECRET_O);
    if (startPressed) {
        if (menu.waitingForStart) pressAnyButtonToStart(menu);
        registerSecretInput(menu, SECRET_START);
    }

    previousPad = pad;
}

#endif
