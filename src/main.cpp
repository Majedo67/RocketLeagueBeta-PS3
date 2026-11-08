
#include "game.cpp"
#include "menu.h"

bool activateMenuOption(Game& game, MenuState& menu) {
    switch (getSelectedMenuOption(menu)) {
        case OPTION_PLAY:
            game.mode = NORMAL;
            resetGame(game);
            menu.visible = false;
            return true;

        case OPTION_HEATSEEKER:
            game.mode = HEATSEEKER;
            resetGame(game);
            menu.visible = false;
            return true;

        case OPTION_BULLET_BALL:
            game.mode = BULLET_BALL;
            resetGame(game);
            menu.visible = false;
            return true;

        case OPTION_TRAINING:
        case OPTION_LOCAL_MATCH:
            game.mode = NORMAL;
            resetGame(game);
            menu.visible = false;
            return true;

        case OPTION_GARAGE:
            menu.screen = MENU_GARAGE;
            return true;

        case OPTION_SETTINGS:
            menu.screen = MENU_SETTINGS;
            return true;

        case OPTION_EXIT:
            return false;

        default:
            return false;
    }
}

int main() {
    Game game{};
    MenuState menu{};

    initializeMenu(menu);

    game.mode = NORMAL;
    resetGame(game);

    return 0;
}
