#include "game.cpp"
#include "audio.h"
#include "controller.h"
#include <sys/timer.h>

bool activateMenuOption(Game& game, MenuState& menu) {
    switch (getSelectedMenuOption(menu)) {
        case OPTION_PLAY:
            game.mode = NORMAL;
            break;

        case OPTION_HEATSEEKER:
            game.mode = HEATSEEKER;
            break;

        case OPTION_BULLET_BALL:
            game.mode = BULLET_BALL;
            break;

        case OPTION_TRAINING:
        case OPTION_LOCAL_MATCH:
            game.mode = NORMAL;
            break;

        case OPTION_GARAGE:
            menu.screen = MENU_GARAGE;
            return true;

        case OPTION_SETTINGS:
            menu.screen = MENU_SETTINGS;
            return true;

        case OPTION_EXIT:
            return false;

        default:
            return true;
    }

    resetGame(game);
    menu.visible = false;
    return true;
}

int main() {
    Game game{};
    MenuState menu{};
    AudioState audio{};

    if (!initializeController())
        return 1;

    initializeMenu(menu);
    resetGame(game);

    initializeAudio(audio, menu);

    bool running = true;
    bool confirm = false;

    while (running) {
        updateController(menu, confirm);

        if (confirm && menu.visible) {
            running = activateMenuOption(game, menu);
        }

        updateAudio(audio, menu);

        if (!menu.visible) {
            updateGame(game, 0.0f, 0.0f, 1.0f / 60.0f);
        }

        sys_timer_usleep(16667);
    }

    shutdownAudio(audio);
    shutdownController();

    return 0;
}