
#include "game.cpp"

int main() {
    Game game{};

    // Start a normal match.
    game.mode = NORMAL;
    resetGame(game);

    // Temporary gameplay test.
    // The real PS3 controller and rendering loop
    // will be added in the next stage.
    const float deltaTime = 1.0f / 60.0f;

    for (int frame = 0; frame < 600; frame++) {
        float throttle = 1.0f;
        float steering = 0.0f;

        updateGame(
            game,
            throttle,
            steering,
            deltaTime
        );
    }

    return 0;
}
