#include "game.cpp"
#include "audio.h"

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

// Gets the music selected by the menu.
const char* getCurrentMusic(const MenuState& menu) {
    return getMusicFile(menu);
}

int main() {
    Game game{};
    MenuState menu{};

    initializeMenu(menu);

    game.mode = NORMAL;
    resetGame(game);

    // Starts with the normal Rocket League music selection.
    const char* musicFile = getCurrentMusic(menu);

    return 0;
}