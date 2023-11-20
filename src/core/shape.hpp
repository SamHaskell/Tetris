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
void shape_render(Context* context, Shape& shape, f32 x, f32 y);
void shape_swap(Shape& a, Shape& b);