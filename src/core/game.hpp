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
    u32 Field[FIELD_WIDTH][FIELD_HEIGHT];
    i32 PlayerX;
    i32 PlayerY;
    Shape ActiveShape;
};

void game_update_and_render(Context* context);