#include "game.cpp"
#include "audio.h"
#include "controller.h"
#include "renderer.h"
#include <sys/timer.h>

int main() {
    Game game{};
    MenuState menu{};
    AudioState audio{};

    if (!initializeController())
        return 1;

    initializeMenu(menu);
    resetGame(game);

    initializeRenderer();
    initializeAudio(audio, menu);

    bool running = true;
    bool confirm = false;

    while (running) {
        updateController(menu, confirm);

        if (confirm && menu.visible) {
            MenuOption option = getSelectedMenuOption(menu);

            if (option == OPTION_EXIT) {
                running = false;
            } else {
                game.mode = option == OPTION_HEATSEEKER
                    ? HEATSEEKER
                    : option == OPTION_BULLET_BALL
                    ? BULLET_BALL
                    : NORMAL;

                resetGame(game);
                menu.visible = false;
            }
        }

        if (!menu.visible) {
            updateGame(game, 0.0f, 0.0f, 1.0f / 60.0f);
        }

        updateAudio(audio, menu);
        renderGame(game, menu);

        sys_timer_usleep(16667);
    }

    shutdownAudio(audio);
    shutdownRenderer();
    shutdownController();

    return 0;
}