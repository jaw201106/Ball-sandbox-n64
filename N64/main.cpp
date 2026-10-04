#include <libdragon.h>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <cstdio>

// N64 Resolution and Memory Constraints
constexpr int SCREEN_WIDTH    = 320;
constexpr int SCREEN_HEIGHT   = 240;
constexpr int MAX_BALLS       = 900;
constexpr float GRAVITY       = 0.15f;
constexpr float BOUNCE        = 0.75f;
constexpr float BALL_RADIUS   = 8.0f;

struct Ball {
    float x, y;
    float vx, vy;
    color_t color;
};

Ball balls[MAX_BALLS];
int ballCount = 0;

float cursorX = SCREEN_WIDTH / 2.0f;
float cursorY = SCREEN_HEIGHT / 2.0f;

color_t GetRGBA(uint8_t r, uint8_t g, uint8_t b) {
    return RGBA32(r, g, b, 255);
}

void SpawnBall(float spawnX, float spawnY) {
    if (ballCount >= MAX_BALLS) return;

    Ball& newBall = balls[ballCount];
    newBall.x = spawnX;
    newBall.y = spawnY;

    newBall.vx = (static_cast<float>(rand() % 40) / 10.0f) - 2.0f;
    newBall.vy = -(static_cast<float>(rand() % 30) / 10.0f);
    newBall.color = GetRGBA(rand() % 180 + 75, rand() % 180 + 75, rand() % 180 + 75);

    ballCount++;
}

void UpdatePhysics() {
    for (int i = 0; i < ballCount; ++i) {
        balls[i].vy += GRAVITY;
        balls[i].x += balls[i].vx;
        balls[i].y += balls[i].vy;

        if (balls[i].x - BALL_RADIUS < 0) {
            balls[i].x = BALL_RADIUS;
            balls[i].vx = -balls[i].vx * BOUNCE;
        } else if (balls[i].x + BALL_RADIUS > SCREEN_WIDTH) {
            balls[i].x = SCREEN_WIDTH - BALL_RADIUS;
            balls[i].vx = -balls[i].vx * BOUNCE;
        }

        if (balls[i].y - BALL_RADIUS < 0) {
            balls[i].y = BALL_RADIUS;
            balls[i].vy = -balls[i].vy * BOUNCE;
        } else if (balls[i].y + BALL_RADIUS > SCREEN_HEIGHT) {
            balls[i].y = SCREEN_HEIGHT - BALL_RADIUS;
            balls[i].vy = -balls[i].vy * BOUNCE;
            balls[i].vx *= 0.98f;
        }

        for (int j = i + 1; j < ballCount; ++j) {
            float dx = balls[j].x - balls[i].x;
            float dy = balls[j].y - balls[i].y;
            float distance = std::sqrt(dx * dx + dy * dy);
            float minDist = BALL_RADIUS * 2.0f;

            if (distance < minDist) {
                if (distance == 0.0f) distance = 0.1f;

                float nx = dx / distance;
                float ny = dy / distance;

                float overlap = minDist - distance;
                balls[i].x -= nx * (overlap * 0.5f);
                balls[i].y -= ny * (overlap * 0.5f);
                balls[j].x += nx * (overlap * 0.5f);
                balls[j].y += ny * (overlap * 0.5f);

                float kx = balls[i].vx - balls[j].vx;
                float ky = balls[i].vy - balls[j].vy;
                float p = 2.0f * (nx * kx + ny * ky) / 2.0f;

                if (p > 0) {
                    balls[i].vx -= p * nx * BOUNCE;
                    balls[i].vy -= p * ny * BOUNCE;
                    balls[j].vx += p * nx * BOUNCE;
                    balls[j].vy += p * ny * BOUNCE;
                }
            }
        }
    }
}

int main() {
    debug_init_isviewer();
    debug_init_usblog();

    display_init(RESOLUTION_320x240, DEPTH_32_BPP, 2, GAMMA_NONE, FILTERS_RESAMPLE);
    rdpq_init();

    // Match API layout where font structure is loaded via rdpq_font but mapped via rdpq_text
    rdpq_font_t *debug_font = rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_VAR);
    rdpq_text_register_font(FONT_BUILTIN_DEBUG_VAR, debug_font);

    joypad_init();

    char uiBuffer[64];

    while (1) {
        joypad_poll();

        joypad_inputs_t inputs = joypad_get_inputs(JOYPAD_PORT_1);
        joypad_buttons_t held = joypad_get_buttons_held(JOYPAD_PORT_1);

        int8_t stickX = inputs.stick_x;
        int8_t stickY = inputs.stick_y;

        if (std::abs(stickX) > 7) cursorX += (stickX / 15.0f);
        if (std::abs(stickY) > 7) cursorY -= (stickY / 15.0f);

        if (cursorX < 0) cursorX = 0;
        if (cursorX > SCREEN_WIDTH) cursorX = SCREEN_WIDTH;
        if (cursorY < 0) cursorY = 0;
        if (cursorY > SCREEN_HEIGHT) cursorY = SCREEN_HEIGHT;

        if (held.a) {
            SpawnBall(cursorX, cursorY);
        }

        UpdatePhysics();

        surface_t* disp = display_get();

        rdpq_attach(disp, NULL);

        rdpq_set_mode_fill(GetRGBA(20, 24, 30));
        rdpq_fill_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

        for (int i = 0; i < ballCount; ++i) {
            rdpq_set_mode_fill(balls[i].color);
            rdpq_fill_rectangle(
                static_cast<int>(balls[i].x - BALL_RADIUS),
                                static_cast<int>(balls[i].y - BALL_RADIUS),
                                static_cast<int>(balls[i].x + BALL_RADIUS),
                                static_cast<int>(balls[i].y + BALL_RADIUS)
            );
        }

        rdpq_set_mode_fill(GetRGBA(255, 255, 255));
        rdpq_fill_rectangle(static_cast<int>(cursorX) - 2, static_cast<int>(cursorY) - 2,
                            static_cast<int>(cursorX) + 2, static_cast<int>(cursorY) + 2);

        std::snprintf(uiBuffer, sizeof(uiBuffer), "Balls: %d / %d (Hold A to Spawn)", ballCount, MAX_BALLS);
        rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_VAR, 10, 20, "%s", uiBuffer);

        rdpq_detach();

        display_show(disp);
    }

    return 0;
}
