
#ifndef NOVA_MENU_H
#define NOVA_MENU_H

enum MenuScreen {
    MENU_HOME,
    MENU_PLAY,
    MENU_GARAGE,
    MENU_SETTINGS
};

enum MenuOption {
    OPTION_PLAY,
    OPTION_HEATSEEKER,
    OPTION_BULLET_BALL,
    OPTION_TRAINING,
    OPTION_LOCAL_MATCH,
    OPTION_GARAGE,
    OPTION_SETTINGS,
    OPTION_EXIT,
    OPTION_COUNT
};

struct MenuState {
    MenuScreen screen;
    int selectedOption;
    bool visible;
};

static const char* MENU_OPTION_NAMES[OPTION_COUNT] = {
    "Play",
    "Heatseeker",
    "Bullet Ball",
    "Training",
    "Local Match",
    "Garage",
    "Settings",
    "Exit"
};

inline void initializeMenu(MenuState& menu) {
    menu.screen = MENU_HOME;
    menu.selectedOption = 0;
    menu.visible = true;
}

inline void moveMenuSelection(MenuState& menu, int direction) {
    if (!menu.visible || direction == 0) return;

    menu.selectedOption += direction;

    if (menu.selectedOption < 0)
        menu.selectedOption = OPTION_COUNT - 1;

    if (menu.selectedOption >= OPTION_COUNT)
        menu.selectedOption = 0;
}

inline MenuOption getSelectedMenuOption(const MenuState& menu) {
    return static_cast<MenuOption>(menu.selectedOption);
}

#endif
