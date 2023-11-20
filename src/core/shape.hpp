#pragma once

#include "core/base.h"
#include "core/platform.hpp"
#include "maths/linalg.hpp"

struct Shape {
    u32 Data[16];
    Vec4 Color;
    u32 ID;
};

void shape_rotate(Shape& shape);
void shape_render(SDL_Renderer* renderer, Shape& shape, i32 x, i32 y);
void shape_swap(Shape& a, Shape& b);