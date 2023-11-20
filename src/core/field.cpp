#include "field.hpp"

#include "core/shape.hpp"

void field_clear(u32* field) {
    memset(field, 0, FIELD_SIZE);
}

void field_set_cell(u32* field, u32 row, u32 col, u32 value) {
    field[(row * FIELD_WIDTH) + col] = value;
}

bool field_check_collision(u32* field, Shape shape, i32 shapeX, i32 shapeY) {
    for (i32 i = 0; i < 4; i++) {
        for (i32 j = 0; j < 4; j++) {
            i32 row = (3 - j) + shapeY;
            i32 col = i + shapeX;
            bool isBoundary = (col >= FIELD_WIDTH || col < 0) || (row < 0);
            bool fieldVal = isBoundary;
            if (row < FIELD_HEIGHT) {
                fieldVal |= (bool)(field[row * FIELD_WIDTH + col]);
            }
            bool collision = (bool)shape.Data[(j * 4) + i] && fieldVal;
            if (collision) { 
                return true;
            }
        }
    }
    return false;
}

void field_place_shape(u32* field, Shape shape, i32 shapeX, i32 shapeY) {
    for (i32 i = 0; i < 4; i++) {
        for (i32 j = 0; j < 4; j++) { 
            i32 row = (3 - j) + shapeY;
            i32 col = i + shapeX; 
            if (shape.Data[(j * 4) + i]) {
                field_set_cell(field, row, col, shape.ID);
            }
        }
    }
}

bool field_check_line(u32* field, u32 row) {
    bool isFull = true;
    for (i32 i = 0; i < FIELD_WIDTH; i++) {
        if (!field[row * FIELD_WIDTH + i]) {
            isFull = false;
            break;
        }
    }
    return isFull;
}