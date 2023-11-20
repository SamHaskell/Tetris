#pragma once

#include "core/base.h"

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 18
#define FIELD_SIZE (FIELD_WIDTH * FIELD_HEIGHT)
#define FIELD_GET_ROW(i) (i32)(i / FIELD_WIDTH)
#define FIELD_GET_COL(i) (i32)(i % FIELD_WIDTH)

struct Shape;

void field_clear(u32* field);
void field_set_cell(u32* field, u32 row, u32 col, u32 value);
bool field_check_collision(u32* field, Shape shape, i32 shapeX, i32 shapeY);
void field_place_shape(u32* field, Shape shape, i32 shapeX, i32 shapeY);
bool field_check_line(u32* field, u32 row);
u32 field_clear_lines(u32* field);