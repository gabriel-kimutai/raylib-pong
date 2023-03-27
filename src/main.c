#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct Ball {
    Vector2 pos;
    float radius;
    Vector2 velocity;
} Ball;

typedef struct Paddle {
    Vector2 pos;
    Vector2 size;
    float velocity;
} Paddle;

typedef struct Score {
    Vector2 pos;
} Score;

int main(int argc, char *argv[])
{
    const int win_width = 800;
    const int win_height = 450;

    InitWindow(win_width, win_height, "Pong");
    Score score = {GetScreenWidth() / 2.0f, 10};

    int score_left = 0;
    int score_right = 0;

    Ball ball = {win_width / 2.0f, win_height / 2.0f, 5.0f};
    ball.velocity = (Vector2){200, 200};

    Paddle paddle_left = {10, (win_height / 2.0f) - 20, 5, 40};
    paddle_left.velocity = 300;
    Rectangle paddle_left_col = {paddle_left.pos.x, paddle_left.pos.y,
                                 paddle_left.size.x, paddle_left.size.y};

    Paddle paddle_right = {win_width - 10, (win_height / 2.0f) - 20, 5, 40};
    paddle_right.velocity = 300;
    Rectangle paddle_right_col = {paddle_right.pos.x, paddle_right.pos.y,
                                  paddle_right.size.x, paddle_right.size.y};

    SetTargetFPS(60);

    bool running = false;
    while (!WindowShouldClose()) {
        // pause && reset mechanic
        if (IsKeyPressed(KEY_I) || IsKeyPressed(KEY_J) || IsKeyPressed(KEY_W) ||
            IsKeyPressed(KEY_S)) {
            running = true;
        }
        if (running && IsKeyPressed(KEY_SPACE)) {
            running = false;
            ball.pos = (Vector2){win_width / 2.0f, win_height / 2.0f};
        }
        if (running) {
            ball.pos.x += ball.velocity.x * GetFrameTime();
            ball.pos.y += ball.velocity.y * GetFrameTime();
            running = true;
        }
        // handle window collision
        if (ball.pos.y > GetScreenHeight()) {
            ball.velocity.y *= -1.0f;
        }
        if (ball.pos.y < 0) {
            ball.velocity.y *= -1.0f;
        }
        if (ball.pos.x > GetScreenWidth()) {
            ball.pos = (Vector2){win_width / 2.0f, win_height / 2.0f};
        }

        // Handle input
        if (IsKeyDown(KEY_W))
            paddle_left.pos.y -= paddle_left.velocity * GetFrameTime();
        if (IsKeyDown(KEY_S))
            paddle_left.pos.y += paddle_left.velocity * GetFrameTime();
        if (IsKeyDown(KEY_I))
            paddle_right.pos.y -= paddle_right.velocity * GetFrameTime();
        if (IsKeyDown(KEY_J))
            paddle_right.pos.y += paddle_right.velocity * GetFrameTime();

        if (CheckCollisionCircleRec(ball.pos, ball.radius, paddle_left_col) ||
            CheckCollisionCircleRec(ball.pos, ball.radius, paddle_right_col)) {
            ball.velocity.x *= -1.1f;
            ball.velocity.y *= -1.1f;
            printf("collision: %s\n", running ? "true" : "false");
        }
        BeginDrawing();
        ClearBackground(BLACK);
        DrawCircleV(ball.pos, ball.radius, WHITE);
        // DrawText("Score:", score.pos.x, score.pos.y, 20, WHITE);
        DrawText(TextFormat("%d : %d", score_left, score_right),
                 score.pos.x - strlen("10 : 10"), score.pos.y, 20, WHITE);

        DrawRectangleV(paddle_left.pos, paddle_left.size, WHITE);
        DrawRectangleV(paddle_right.pos, paddle_right.size, WHITE);
        DrawLineV((Vector2){GetScreenWidth() / 2.0f, 0},
                  (Vector2){GetScreenWidth() / 2.0f, GetScreenHeight()},
                  RAYWHITE);
        DrawLineV(paddle_right.pos, ball.pos, GREEN);

        if (ball.pos.x > paddle_right.pos.x) {
            score_left = score_left;
        }
        if (ball.pos.x < paddle_left.pos.x) {
            score_right += 1;
        }

        // paddle and ball collision detection

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
