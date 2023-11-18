#include "maths/random.hpp"

static u32 s_Seed;

f32 RandFloat() {
    return RandFloat(s_Seed);
}

f32 RandFloat(f32 min, f32 max) {
    return RandFloat(s_Seed, min, max);
}

u32 RandU32() {
    return RandU32(s_Seed);
}

u32 RandU32(u32 min, u32 max) {
    return RandU32(s_Seed, min, max);
}

Vec2 RandVec2() {
    return RandVec2(s_Seed);
}

Vec3 RandVec3() {
    return RandVec3(s_Seed);
}

Mat2x2 RandMat2() {
    return RandMat2(s_Seed);
}

Mat3x3 RandMat3() {
    return RandMat3(s_Seed);
}