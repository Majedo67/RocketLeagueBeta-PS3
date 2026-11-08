
#include "game.h"
#include <cmath>

float clampValue(float v, float low, float high) {
    if (v < low) return low;
    if (v > high) return high;
    return v;
}

float vecLength(Vec3 v) {
    return std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

Vec3 normalizeVec(Vec3 v) {
    float len = vecLength(v);
    if (len < 0.001f) return {0, 0, 0};
    return {v.x/len, v.y/len, v.z/len};
}

void resetBall(Game& game) {
    game.ball.position = {0, 1.5f, 0};
    game.ball.velocity = {0, 0, 0};
    game.ball.speed = 0;
    game.ball.lastHitTeam = -1;
    game.ball.targetTeam = 1;
}

void resetGame(Game& game) {
    game.blueScore = 0;
    game.orangeScore = 0;
    game.matchTime = 0;

    game.car.position = {0, 1, -15};
    game.car.velocity = {0, 0, 0};
    game.car.rotation = 0;
    game.car.boost = 100;
    game.car.team = 0;
    game.car.jumping = false;

    resetBall(game);
}

void moveCar(Game& game, float throttle,
             float steering, float dt) {
    Car& car = game.car;

    float speed = vecLength(car.velocity);
    float steeringPower = 2.4f / (1.0f + speed * 0.04f);

    car.rotation += steering * steeringPower * dt;

    Vec3 forward = {
        std::sin(car.rotation),
        0,
        std::cos(car.rotation)
    };

    car.velocity.x += forward.x * throttle * 24.0f * dt;
    car.velocity.z += forward.z * throttle * 24.0f * dt;

    car.velocity.x *= 0.992f;
    car.velocity.z *= 0.992f;

    float currentSpeed = vecLength(car.velocity);
    if (currentSpeed > 27.0f) {
        car.velocity.x *= 27.0f / currentSpeed;
        car.velocity.z *= 27.0f / currentSpeed;
    }

    car.position.x += car.velocity.x * dt;
    car.position.z += car.velocity.z * dt;

    car.position.x = clampValue(
        car.position.x, -ARENA_X + 2, ARENA_X - 2
    );
    car.position.z = clampValue(
        car.position.z, -ARENA_Z + 2, ARENA_Z - 2
    );
}

void jumpCar(Game& game) {
    if (!game.car.jumping) {
        game.car.velocity.y = 9.0f;
        game.car.jumping = true;
    }
}

void updateCarVertical(Car& car, float dt) {
    car.velocity.y -= 20.0f * dt;
    car.position.y += car.velocity.y * dt;

    if (car.position.y <= 1.0f) {
        car.position.y = 1.0f;
        car.velocity.y = 0;
        car.jumping = false;
    }
}

void hitBall(Game& game, int team) {
    Vec3 difference = {
        game.ball.position.x - game.car.position.x,
        game.ball.position.y - game.car.position.y,
        game.ball.position.z - game.car.position.z
    };

    float distance = vecLength(difference);

    if (distance > BALL_RADIUS + CAR_RADIUS ||
        distance < 0.001f)
        return;

    Vec3 direction = normalizeVec(difference);

    float impact =
        game.car.velocity.x * direction.x +
        game.car.velocity.y * direction.y +
        game.car.velocity.z * direction.z;

    game.ball.velocity.x += direction.x * (impact + 8.0f);
    game.ball.velocity.y += direction.y * (impact + 5.0f);
    game.ball.velocity.z += direction.z * (impact + 8.0f);

    game.ball.lastHitTeam = team;

    if (game.mode == HEATSEEKER) {
        game.ball.targetTeam = 1 - team;
    }
}

void updateBall(Game& game, float dt) {
    Ball& ball = game.ball;

    if (game.mode == HEATSEEKER) {
        float targetZ = ball.targetTeam == 0
            ? -ARENA_Z : ARENA_Z;

        Vec3 targetDirection = normalizeVec({
            -ball.position.x * 0.15f,
            0,
            targetZ - ball.position.z
        });

        ball.velocity.x += targetDirection.x * 5.0f * dt;
        ball.velocity.z += targetDirection.z * 5.0f * dt;
        ball.speed += 1.5f * dt;
    }

    ball.velocity.y -= 12.0f * dt;

    ball.position.x += ball.velocity.x * dt;
    ball.position.y += ball.velocity.y * dt;
    ball.position.z += ball.velocity.z * dt;

    if (ball.position.y < BALL_RADIUS) {
        ball.position.y = BALL_RADIUS;
        ball.velocity.y = std::fabs(ball.velocity.y) * 0.72f;
    }

    if (ball.position.x < -ARENA_X ||
        ball.position.x > ARENA_X) {
        ball.position.x = clampValue(
            ball.position.x, -ARENA_X, ARENA_X
        );
        ball.velocity.x *= -0.85f;
    }

    if (ball.position.z < -ARENA_Z ||
        ball.position.z > ARENA_Z) {
        ball.position.z = clampValue(
            ball.position.z, -ARENA_Z, ARENA_Z
        );
        ball.velocity.z *= -0.85f;
    }

    if (game.mode == BULLET_BALL) {
        ball.velocity.x *= 1.003f;
        ball.velocity.z *= 1.003f;
    }

    float speed = vecLength(ball.velocity);
    float maxSpeed = game.mode == HEATSEEKER ? 45.0f : 35.0f;

    if (speed > maxSpeed) {
        float scale = maxSpeed / speed;
        ball.velocity.x *= scale;
        ball.velocity.y *= scale;
        ball.velocity.z *= scale;
    }
}

void updateGame(Game& game, float throttle,
                float steering, float dt) {
    if (dt <= 0 || dt > 0.05f) return;

    game.matchTime += dt;

    moveCar(game, throttle, steering, dt);
    updateCarVertical(game.car, dt);
    updateBall(game, dt);
    hitBall(game, game.car.team);
}
