#include "game.h"
#include <cmath>

static const float PI = 3.14159265f;
static const float GOAL_HALF_WIDTH = 10.0f;
static const float GOAL_HEIGHT = 8.0f;

float clampValue(float v, float low, float high) {
    return v < low ? low : (v > high ? high : v);
}

float vecLength(Vec3 v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vec3 normalizeVec(Vec3 v) {
    float length = vecLength(v);
    if (length <= 0.0001f) return {0.0f, 0.0f, 0.0f};
    return {v.x / length, v.y / length, v.z / length};
}

void resetBall(Game& game) {
    game.ball.position = {0.0f, BALL_RADIUS, 0.0f};
    game.ball.velocity = {0.0f, 0.0f, 0.0f};
    game.ball.targetTeam = 1;
    game.ball.lastHitTeam = -1;
    game.ball.speed = 0.0f;
}

void resetGame(Game& game) {
    game.car.position = {0.0f, 0.0f, -20.0f};
    game.car.velocity = {0.0f, 0.0f, 0.0f};
    game.car.rotation = 0.0f;
    game.car.boost = 100.0f;
    game.car.team = 0;
    game.car.jumping = false;
    game.car.boosting = false;
    game.car.drifting = false;
    game.mode = NORMAL;
    game.blueScore = 0;
    game.orangeScore = 0;
    game.matchTime = 300.0f;
    resetBall(game);
}

void moveCar(Game& game, float throttle, float steering, float dt) {
    Car& car = game.car;
    throttle = clampValue(throttle, -1.0f, 1.0f);
    steering = clampValue(steering, -1.0f, 1.0f);

    const bool useBoost = car.boosting && car.boost > 0.0f;
    const float acceleration = useBoost ? 43.0f : 24.0f;
    const float turnRate = car.drifting ? 2.65f : 1.75f;

    car.rotation += steering * turnRate * dt;
    if (car.rotation > PI) car.rotation -= 2.0f * PI;
    if (car.rotation < -PI) car.rotation += 2.0f * PI;

    const float forwardX = std::sin(car.rotation);
    const float forwardZ = std::cos(car.rotation);
    car.velocity.x += forwardX * throttle * acceleration * dt;
    car.velocity.z += forwardZ * throttle * acceleration * dt;

    if (useBoost) car.boost = clampValue(car.boost - 28.0f * dt, 0.0f, 100.0f);
    else car.boost = clampValue(car.boost + 9.0f * dt, 0.0f, 100.0f);

    const float maxSpeed = useBoost ? 35.0f : 22.0f;
    const float speed = std::sqrt(car.velocity.x * car.velocity.x +
                                  car.velocity.z * car.velocity.z);
    if (speed > maxSpeed) {
        car.velocity.x *= maxSpeed / speed;
        car.velocity.z *= maxSpeed / speed;
    }

    const float drag = car.drifting ? 0.45f : 1.65f;
    const float dragFactor = clampValue(1.0f - drag * dt, 0.0f, 1.0f);
    car.velocity.x *= dragFactor;
    car.velocity.z *= dragFactor;

    car.position.x += car.velocity.x * dt;
    car.position.z += car.velocity.z * dt;
    car.position.x = clampValue(car.position.x, -ARENA_X + CAR_RADIUS, ARENA_X - CAR_RADIUS);
    car.position.z = clampValue(car.position.z, -ARENA_Z + CAR_RADIUS, ARENA_Z - CAR_RADIUS);
}

void jumpCar(Game& game) {
    Car& car = game.car;
    if (!car.jumping) {
        car.velocity.y = 12.0f;
        car.jumping = true;
    }
}

void updateCarVertical(Car& car, float dt) {
    if (!car.jumping) return;
    car.velocity.y -= 24.0f * dt;
    car.position.y += car.velocity.y * dt;
    if (car.position.y <= 0.0f) {
        car.position.y = 0.0f;
        car.velocity.y = 0.0f;
        car.jumping = false;
    }
}

void hitBall(Game& game, int team) {
    Vec3 direction = normalizeVec({
        game.ball.position.x - game.car.position.x,
        game.ball.position.y - (game.car.position.y + 1.0f),
        game.ball.position.z - game.car.position.z
    });
    if (vecLength(direction) < 0.001f) direction = {0.0f, 0.2f, 1.0f};
    game.ball.velocity.x += direction.x * 13.0f;
    game.ball.velocity.y += direction.y * 13.0f + 2.0f;
    game.ball.velocity.z += direction.z * 13.0f;
    game.ball.lastHitTeam = team;
    game.ball.targetTeam = team == 0 ? 1 : 0;
}

static void collideCarAndBall(Game& game) {
    Vec3 delta = {
        game.ball.position.x - game.car.position.x,
        game.ball.position.y - (game.car.position.y + 1.0f),
        game.ball.position.z - game.car.position.z
    };
    const float distance = vecLength(delta);
    const float minimum = BALL_RADIUS + CAR_RADIUS * 0.65f;
    if (distance > 0.0001f && distance < minimum) {
        Vec3 normal = normalizeVec(delta);
        const float overlap = minimum - distance;
        game.ball.position.x += normal.x * overlap;
        game.ball.position.y += normal.y * overlap;
        game.ball.position.z += normal.z * overlap;

        const float closingSpeed =
            (game.car.velocity.x - game.ball.velocity.x) * normal.x +
            (game.car.velocity.y - game.ball.velocity.y) * normal.y +
            (game.car.velocity.z - game.ball.velocity.z) * normal.z;
        if (closingSpeed > 0.0f) hitBall(game, game.car.team);
    }
}

void updateBall(Game& game, float dt) {
    Ball& ball = game.ball;
    ball.velocity.y -= 18.0f * dt;

    if (game.mode == HEATSEEKER) {
        const float targetZ = ball.targetTeam == 0 ? -ARENA_Z : ARENA_Z;
        Vec3 aim = normalizeVec({-ball.position.x, 0.0f, targetZ - ball.position.z});
        ball.velocity.x += aim.x * 7.0f * dt;
        ball.velocity.z += aim.z * 7.0f * dt;
    }
    if (game.mode == BULLET_BALL) {
        ball.velocity.x *= 1.0f + 0.35f * dt;
        ball.velocity.z *= 1.0f + 0.35f * dt;
    }

    ball.position.x += ball.velocity.x * dt;
    ball.position.y += ball.velocity.y * dt;
    ball.position.z += ball.velocity.z * dt;

    if (ball.position.y < BALL_RADIUS) {
        ball.position.y = BALL_RADIUS;
        ball.velocity.y = std::fabs(ball.velocity.y) * 0.72f;
        ball.velocity.x *= 0.985f;
        ball.velocity.z *= 0.985f;
    }

    if (ball.position.x > ARENA_X - BALL_RADIUS ||
        ball.position.x < -ARENA_X + BALL_RADIUS) {
        ball.position.x = clampValue(ball.position.x, -ARENA_X + BALL_RADIUS, ARENA_X - BALL_RADIUS);
        ball.velocity.x = -ball.velocity.x * 0.82f;
    }

    const bool insideGoal = std::fabs(ball.position.x) < GOAL_HALF_WIDTH &&
                            ball.position.y < GOAL_HEIGHT;
    if (insideGoal && ball.position.z > ARENA_Z) {
        ++game.blueScore;
        resetBall(game);
        return;
    }
    if (insideGoal && ball.position.z < -ARENA_Z) {
        ++game.orangeScore;
        resetBall(game);
        return;
    }

    if (ball.position.z > ARENA_Z || ball.position.z < -ARENA_Z) {
        ball.position.z = clampValue(ball.position.z, -ARENA_Z, ARENA_Z);
        ball.velocity.z = -ball.velocity.z * 0.82f;
    }
    ball.speed = vecLength(ball.velocity);
}

void updateGame(Game& game, float throttle, float steering, float dt) {
    if (dt <= 0.0f) return;
    moveCar(game, throttle, steering, dt);
    updateCarVertical(game.car, dt);
    collideCarAndBall(game);
    updateBall(game, dt);
    if (game.matchTime > 0.0f) {
        game.matchTime -= dt;
        if (game.matchTime < 0.0f) game.matchTime = 0.0f;
    }
}
