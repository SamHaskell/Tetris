#pragma once

#include "core/base.h"
#include "core/field.hpp"
#include "core/shape.hpp"
#include "core/input.hpp"

#include "soloud_wav.h"

struct Context;

#define FONT_SIZE_LARGE 36
#define FONT_SIZE_MEDIUM 24
#define FONT_SIZE_SMALL 16

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

    SoLoud::Wav BGM;
    SoLoud::Wav KickSFX;

    GameState GameState;
    
    u32 Field[FIELD_SIZE];
    i32 PlayerX;
    i32 PlayerY;
    
    Shape CurrentShape;
    Shape NextShape;
    bool CanSwap;
    
    f64 DeltaTime = 0.0;
    f64 ElapsedGameTime = 0.0;
    f64 ElapsedSinceLastMoveDown = 0.0;
    f64 TimeToMoveDown = 0.8;
    f64 LockTime = 0.0;
    f64 LockDelay = 0.5;
};

void game_init(Context* context);
void game_shutdown(Context* context);
void game_update_and_render(Context* context, f64 dt);