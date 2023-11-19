#pragma once

#include "core/base.h"
#include "core/shape.hpp"

struct Context;

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 18
#define FIELD_SIZE (FIELD_WIDTH * FIELD_HEIGHT)
#define FIELD_GET_ROW(i) (i32)(i / FIELD_WIDTH)
#define FIELD_GET_COL(i) (i32)(i % FIELD_WIDTH)

struct GameState {
    TTF_Font* MainFont;
    u32 Field[FIELD_HEIGHT][FIELD_WIDTH];
    i32 PlayerX;
    i32 PlayerY;
    Shape ActiveShape;
    f64 DeltaTime = 0.0;
    f64 ElapsedGameTime = 0.0;
    f64 ElapsedSinceLastMoveDown = 0.0;
    f64 TimeToMoveDown = 0.5;
};

void game_init(Context* context);
void game_update_and_render(Context* context);