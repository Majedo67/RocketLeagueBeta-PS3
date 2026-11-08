
#include "game.h"
#include "menu.h"
#include "audio.h"
#include "controller.h"
#include "renderer.h"
#include <sys/timer.h>

int main() {
    Game game{};
    MenuState menu{};
    AudioState audio{};

    float throttle = 0.0f;
    float steering = 0.0f;

    bool running = true;
    bool confirm = false;

    if (!initializeController())
        return 1;

    initializeMenu(menu);
    resetGame(game);

    initializeRenderer();
    initializeAudio(audio, menu);

    while (running) {
        updateController(
            menu,
            game,
            confirm,
            throttle,
            steering
        );

        if (confirm && menu.visible &&
            !menu.waitingForStart) {

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
            updateGame(
                game,
                throttle,
                steering,
                1.0f / 60.0f
            );
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
