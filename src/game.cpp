#include "game.h"
#include <cmath>

float clampValue(float v, float low, float high) {
    if (v < low) return low;
    if (v > high) return high;
    return v;
}

float vecLength(Vec3 v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vec3 normalizeVec(Vec3 v) {
    float length = vecLength(v);

    if (length <= 0.0001f)
        return {0.0f, 0.0f, 0.0f};

    return {
        v.x / length,
        v.y / length,
        v.z / length
    };
}

void resetBall(Game& game) {
    game.ball.position = {0.0f, 1.5f, 0.0f};
    game.ball.velocity = {0.0f, 0.0f, 0.0f};
    game.ball.targetTeam = 0;
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

    float acceleration = car.boosting ? 45.0f : 24.0f;

    car.rotation += steering *
        (car.drifting ? 2.8f : 1.8f) * dt;

    float forwardX = std::sin(car.rotation);
    float forwardZ = std::cos(car.rotation);

    car.velocity.x += forwardX * throttle * acceleration * dt;
    car.velocity.z += forwardZ * throttle * acceleration * dt;

    if (car.boosting && car.boost > 0.0f) {
        car.boost -= 25.0f * dt;
        if (car.boost < 0.0f) car.boost = 0.0f;
    }

    if (!car.boosting && car.boost < 100.0f) {
        car.boost += 8.0f * dt;
        if (car.boost > 100.0f) car.boost = 100.0f;
    }

    float maxSpeed = car.boosting ? 35.0f : 22.0f;
    float speed = std::sqrt(
        car.velocity.x * car.velocity.x +
        car.velocity.z * car.velocity.z
    );

    if (speed > maxSpeed) {
        car.velocity.x *= maxSpeed / speed;
        car.velocity.z *= maxSpeed / speed;
    }

    float friction = car.drifting ? 0.35f : 1.8f;

    if (!car.jumping) {
        car.velocity.x *= 1.0f - friction * dt;
        car.velocity.z *= 1.0f - friction * dt;
    }

    car.position.x += car.velocity.x * dt;
    car.position.z += car.velocity.z * dt;

    car.position.x = clampValue(
        car.position.x, -ARENA_X, ARENA_X
    );

    car.position.z = clampValue(
        car.position.z, -ARENA_Z, ARENA_Z
    );
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
        0.0f,
        game.ball.position.z - game.car.position.z
    });

    game.ball.velocity.x += direction.x * 12.0f;
    game.ball.velocity.z += direction.z * 12.0f;

    game.ball.lastHitTeam = team;
    game.ball.targetTeam = team == 0 ? 1 : 0;
}

void updateBall(Game& game, float dt) {
    Ball& ball = game.ball;

    ball.velocity.y -= 18.0f * dt;

    if (game.mode == HEATSEEKER) {
        float targetZ = ball.targetTeam == 0
            ? -ARENA_Z : ARENA_Z;

        float direction = targetZ > ball.position.z
            ? 1.0f : -1.0f;

        ball.velocity.z += direction * 5.0f * dt;
    }

    if (game.mode == BULLET_BALL) {
        ball.velocity.x *= 1.01f;
        ball.velocity.z *= 1.01f;
    }

    ball.position.x += ball.velocity.x * dt;
    ball.position.y += ball.velocity.y * dt;
    ball.position.z += ball.velocity.z * dt;

    if (ball.position.y < BALL_RADIUS) {
        ball.position.y = BALL_RADIUS;
        ball.velocity.y = -ball.velocity.y * 0.7f;
    }

    if (ball.position.x > ARENA_X ||
        ball.position.x < -ARENA_X) {
        ball.velocity.x = -ball.velocity.x * 0.8f;
    }

    if (ball.position.z > ARENA_Z ||
        ball.position.z < -ARENA_Z) {
        ball.velocity.z = -ball.velocity.z * 0.8f;
    }
}

void updateGame(
    Game& game,
    float throttle,
    float steering,
    float dt
) {
    moveCar(game, throttle, steering, dt);
    updateCarVertical(game.car, dt);
    updateBall(game, dt);

    if (game.matchTime > 0.0f)
        game.matchTime -= dt;
}