#include "core/shape.hpp"

/*
    Rotates a shape clockwise in-place
*/

void shape_rotate(Shape& shape) {
    u32 rotatedData[16] = {};
    for (i32 i = 0; i < 4; i++) {
        for (i32 j = 0; j < 4; j++) {
            rotatedData[j * 4 + (3 - i)] = shape.Data[i * 4 + j];
        }
    }
    memcpy(&shape.Data, &rotatedData, 16 * sizeof(u32));
}

/*
    Draws the 128 x 128 shape, where (x, y) is the bottom-left corner
*/

void shape_render(Context* context, Shape& shape, f32 x, f32 y) {
    for (i32 j = 0; j < 4; j++) {
        for (i32 i = 0; i < 4; i++) {
            if (shape.Data[(j * 4) + i]) {
                Rect2D rect = {x + (f32)(i * 32), y + (f32)(96 - (j * 32)), 32, 32};
                draw_quad_filled(context, shape.Color, rect);
                draw_quad_outline(context, {0.0, 0.0, 0.0, 1.0}, rect);
            }
        }
    }
}