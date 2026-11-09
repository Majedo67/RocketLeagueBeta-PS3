#ifndef GAME_H
#define GAME_H

const float ARENA_X = 40.0f;
const float ARENA_Z = 60.0f;
const float BALL_RADIUS = 1.5f;
const float CAR_RADIUS = 2.0f;

enum GameMode {
    NORMAL,
    HEATSEEKER,
    BULLET_BALL
};

struct Vec3 {
    float x, y, z;
};

struct Car {
    Vec3 position;
    Vec3 velocity;
    float rotation;
    float boost;
    int team;
    bool jumping;
    bool boosting;
    bool drifting;
};

struct Ball {
    Vec3 position;
    Vec3 velocity;
    int targetTeam;
    int lastHitTeam;
    float speed;
};

struct Game {
    Car car;
    Ball ball;
    GameMode mode;
    int blueScore;
    int orangeScore;
    float matchTime;
};

float clampValue(float v, float low, float high);
float vecLength(Vec3 v);
Vec3 normalizeVec(Vec3 v);

void resetBall(Game& game);
void resetGame(Game& game);
void moveCar(Game& game, float throttle, float steering, float dt);
void jumpCar(Game& game);
void updateCarVertical(Car& car, float dt);
void hitBall(Game& game, int team);
void updateBall(Game& game, float dt);
void updateGame(Game& game, float throttle, float steering, float dt);

#endif