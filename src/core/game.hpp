#pragma once

#include "core/base.h"
#include "core/field.hpp"
#include "core/shape.hpp"
#include "core/input.hpp"

#include "soloud_wav.h"
#include "soloud_wavstream.h"

struct Context;

#define FONT_SIZE_LARGE 36
#define FONT_SIZE_MEDIUM 24
#define FONT_SIZE_SMALL 16

#define COLOR_BACKGROUND {0.976, 0.90, 0.830, 1.0}
#define COLOR_WALLS {0.676, 0.50, 0.430, 1.0}
#define COLOR_OVERLAY {0.18, 0.1, 0.14, 0.8}

#define COLOR_TEXTBACKGROUND {0.8, 0.8, 0.8, 0.2}
#define COLOR_TEXT_DARK {0.1, 0.1, 0.16, 1.0}
#define COLOR_TEXT_LIGHT {0.9, 0.9, 0.84, 1.0}

#define COLOR_ACCENT {0.676, 0.50, 0.430, 1.0}

// Time it takes for the piece to move down one row when no inputs are pressed at the start of the game.
#define INIT_DROP_TIME 0.8

// Time it takes for the piece to move down one row when down is held.
#define QUICK_DROP_TIME 0.1

// Time it takes for the piece to slide to the side one col when left/right is held.
#define QUICK_SLIDE_TIME 0.1

#define MIN_BGM_VOLUME 0.6
#define MAX_BGM_VOLUME 1.0

enum class GameState {
    Start,
    Paused,
    Playing,
    GameOver
};

struct Game {
    TTF_Font* MainFontLarge;
    TTF_Font* MainFontMedium;
    TTF_Font* MainFontSmall;

    SoLoud::handle BGMHandle;
    SoLoud::WavStream BGM;
    SoLoud::Wav KickSFX;

    GameState GameState;
    
    u32 Field[FIELD_SIZE];
    i32 PlayerX;
    i32 PlayerY;
    
    Shape CurrentShape;
    Shape NextShape;
    bool CanSwap;

    /*
        TODO: Implement soft-locking.
    */

    // bool IsSoftLocked;
    // f64 LockTime = 0.0;
    // f64 LockDelay = 0.5;

    f64 ElapsedGameTime = 0.0;
    f64 ElapsedSinceLastMoveDown = 0.0;
    f64 ElapsedSinceLastSlide = 0.0;
    f64 TimeToMoveDown = INIT_DROP_TIME;
};

void game_init(Context* context);
void game_shutdown(Context* context);
void game_update_and_render(Context* context, f64 dt);

void game_over(Context* context);
void game_restart(Context* context);