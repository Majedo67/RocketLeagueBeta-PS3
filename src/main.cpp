#include "game.h"
#include "menu.h"
#include "controller.h"
#include "renderer.h"

int main() {
    Game game{};
    MenuState menu{};

    bool confirm = false;
    float throttle = 0.0f;
    float steering = 0.0f;

    resetGame(game);
    initializeMenu(menu);

    if (!initializeController()) {
        return 1;
    }

    initializeRenderer();

    const float dt = 1.0f / 60.0f;

    while (true) {
        updateController(
            menu,
            game,
            confirm,
            throttle,
            steering
        );

        if (confirm) {
            menu.visible = false;
        }

        if (!menu.visible) {
            updateGame(game, throttle, steering, dt);
        }

        renderGame(game, menu);
    }

    shutdownRenderer();
    shutdownController();

    return 0;
}