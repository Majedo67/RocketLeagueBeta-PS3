#ifndef RENDERER_H
#define RENDERER_H

#include <tiny3d.h>
#include "game.cpp"
#include "menu.h"

inline void initializeRenderer() {
    tiny3d_Init(1024 * 1024);
}

inline void drawRect(
    float x, float y, float w, float h,
    unsigned int color
) {
    tiny3d_SetPolygon(TINY3D_QUADS);

    tiny3d_VertexPos(x, y, 65535);
    tiny3d_VertexColor(color);

    tiny3d_VertexPos(x + w, y, 65535);
    tiny3d_VertexPos(x + w, y + h, 65535);
    tiny3d_VertexPos(x, y + h, 65535);

    tiny3d_End();
}

inline void renderGame(
    const Game& game,
    const MenuState& menu
) {
    tiny3d_Clear(0xFF080D20, TINY3D_CLEAR_ALL);
    tiny3d_Project2D();

    // Arena background
    drawRect(0, 0, 848, 512, 0xFF101B35);

    // Blue and orange team areas
    drawRect(0, 230, 100, 50, 0xFF1677FFFF);
    drawRect(748, 230, 100, 50, 0xFFFF8A20FF);

    // Center line
    drawRect(422, 40, 4, 432, 0xFFFFFFFF);

    // Ball
    drawRect(
        420 + game.ball.position.x * 2,
        250 - game.ball.position.z * 2,
        12, 12,
        0xFFFFFFFF
    );

    if (menu.visible) {
        // Menu panel
        drawRect(220, 100, 408, 310, 0xEE07152FFF);

        // Selected option highlight
        drawRect(
            260,
            155 + menu.selectedOption * 32,
            328,
            25,
            0xFF087FFFFF
        );
    } else {
        // Player car
        drawRect(
            410 + game.car.position.x * 2,
            250 - game.car.position.z * 2,
            28, 18,
            0xFF1688FFFF
        );
    }

    tiny3d_Flip();
}

inline void shutdownRenderer() {
    tiny3d_Exit();
}

#endif