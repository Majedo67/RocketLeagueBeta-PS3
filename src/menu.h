
#ifndef NOVA_MENU_H
#define NOVA_MENU_H

enum MenuScreen {
    MENU_TITLE,
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

enum SecretInput {
    SECRET_UP,
    SECRET_DOWN,
    SECRET_LEFT,
    SECRET_RIGHT,
    SECRET_X,
    SECRET_O,
    SECRET_START,
    SECRET_OTHER
};

enum MusicTrack {
    MUSIC_NORMAL,
    MUSIC_SARPBC
};

struct MenuState {
    MenuScreen screen;
    int selectedOption;
    bool visible;
    bool waitingForStart;
    bool sarpbcUnlocked;
    int secretCodeProgress;
    MusicTrack music;
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

static const SecretInput SARPBC_CODE[11] = {
    SECRET_UP, SECRET_UP,
    SECRET_DOWN, SECRET_DOWN,
    SECRET_LEFT, SECRET_RIGHT,
    SECRET_LEFT, SECRET_RIGHT,
    SECRET_X, SECRET_O,
    SECRET_START
};

inline void initializeMenu(MenuState& menu) {
    menu.screen = MENU_TITLE;
    menu.selectedOption = 0;
    menu.visible = true;
    menu.waitingForStart = true;
    menu.sarpbcUnlocked = false;
    menu.secretCodeProgress = 0;
    menu.music = MUSIC_NORMAL;
}

inline void pressAnyButtonToStart(MenuState& menu) {
    if (menu.waitingForStart) {
        menu.waitingForStart = false;
        menu.screen = MENU_HOME;
    }
}

inline bool registerSecretInput(
    MenuState& menu,
    SecretInput input
) {
    if (menu.sarpbcUnlocked) return true;

    if (input == SARPBC_CODE[menu.secretCodeProgress]) {
        menu.secretCodeProgress++;

        if (menu.secretCodeProgress == 11) {
            menu.sarpbcUnlocked = true;
            menu.music = MUSIC_SARPBC;
            menu.secretCodeProgress = 0;
            return true;
        }
    } else {
        menu.secretCodeProgress =
            (input == SECRET_UP) ? 1 : 0;
    }

    return false;
}

inline void setNormalMusic(MenuState& menu) {
    menu.music = MUSIC_NORMAL;
}

inline void moveMenuSelection(
    MenuState& menu,
    int direction
) {
    if (!menu.visible ||
        menu.waitingForStart ||
        direction == 0)
        return;

    menu.selectedOption += direction;

    if (menu.selectedOption < 0)
        menu.selectedOption = OPTION_COUNT - 1;

    if (menu.selectedOption >= OPTION_COUNT)
        menu.selectedOption = 0;
}

inline MenuOption getSelectedMenuOption(
    const MenuState& menu
) {
    return static_cast<MenuOption>(
        menu.selectedOption
    );
}

#endif
