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

    // Keep the game running even if controller initialization fails.
    // updateController() safely returns when no pad data is available.
    const bool controllerReady = initializeController();

    // tiny3d_Init initializes the PS3 video/RSX environment.
    if (tiny3d_Init(1024 * 1024) != TINY3D_OK) {
        if (controllerReady) shutdownController();
        return 1;
    }

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
    if (controllerReady) shutdownController();
    return 0;
}
