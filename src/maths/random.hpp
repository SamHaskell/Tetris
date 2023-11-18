#pragma once

#include "core/base.h"
#include "core/utils.hpp"
#include "maths/numerics.hpp"
#include "maths/linalg.hpp"

f32 RandFloat();
f32 RandFloat(f32 min, f32 max);
u32 RandU32();
u32 RandU32(u32 min, u32 max);
Vec2 RandVec2();
Vec3 RandVec3();
Mat2x2 RandMat2();
Mat3x3 RandMat3();

inline f32 RandFloat(u32& seed) {
    seed = Utils::HashPCG(seed);
    return (f32)seed / (f32)UINT32_MAX;
}

inline f32 RandFloat(u32& seed, f32 min, f32 max) {
    return Lerp(min, max, RandFloat(seed));
}

inline u32 RandU32(u32& seed) {
    seed = Utils::HashPCG(seed);
    return seed;
}

inline u32 RandU32(u32& seed, u32 min, u32 max) {
    return min + (RandU32(seed) % (max - min + 1));
}

inline Vec2 RandVec2(u32& seed) {
    return { RandFloat(seed), RandFloat(seed) };
}

inline Vec3 RandVec3(u32& seed) {
    return { RandFloat(seed), RandFloat(seed), RandFloat(seed) };
}

inline Mat2x2 RandMat2(u32& seed) {
    return {
        RandFloat(seed), RandFloat(seed),
        RandFloat(seed), RandFloat(seed),
    };
}

inline Mat3x3 RandMat3(u32& seed) {
    return {
        RandFloat(seed), RandFloat(seed), RandFloat(seed),
        RandFloat(seed), RandFloat(seed), RandFloat(seed),
        RandFloat(seed), RandFloat(seed), RandFloat(seed),
    };
}