#include "game.h"
#include "menu.h"
#include "controller.h"
#include "renderer.h"

int main() {
    Game game = {};
    MenuState menu = {};
    bool confirm = false;
    float throttle = 0.0f;
    float steering = 0.0f;

    resetGame(game);
    initializeMenu(menu);

    if (!initializeController()) return 1;
    initializeRenderer();

    const float dt = 1.0f / 60.0f;

    while (true) {
        updateController(menu, game, confirm, throttle, steering);

        if (confirm && !menu.waitingForStart) {
            switch (getSelectedMenuOption(menu)) {
                case OPTION_PLAY:
                case OPTION_LOCAL_MATCH:
                    game.mode = NORMAL;
                    menu.visible = false;
                    break;
                case OPTION_HEATSEEKER:
                    game.mode = HEATSEEKER;
                    menu.visible = false;
                    break;
                case OPTION_BULLET_BALL:
                    game.mode = BULLET_BALL;
                    menu.visible = false;
                    break;
                case OPTION_TRAINING:
                    game.mode = NORMAL;
                    game.matchTime = 0.0f;
                    menu.visible = false;
                    break;
                case OPTION_GARAGE:
                    menu.screen = MENU_GARAGE;
                    break;
                case OPTION_SETTINGS:
                    menu.screen = MENU_SETTINGS;
                    break;
                case OPTION_EXIT:
                default:
                    break;
            }
        }

        if (!menu.visible) updateGame(game, throttle, steering, dt);
        renderGame(game, menu);
    }

    shutdownRenderer();
    shutdownController();
    return 0;
}
