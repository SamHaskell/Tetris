#pragma once

#include "core/base.h"

/*
    NOTE: This is a first pass to get enough functionality going that i can test correctness and
    generally get going with other things. We pass basically everything Vec4 and smaller by value where possible 
    for simplicity, need to profile to see if this is faster than passing by reference.
*/

struct Vec2;
struct Vec3;
struct Vec4;

struct Mat2x2;
struct Mat3x3;
struct Mat4x4;

struct Vec2 {
    Vec2(f32 x, f32 y) : x(x), y(y) {}
    Vec2() {}

    f32 x;
    f32 y;

    void operator*=(const f32 val) { x *= val; y *= val; }
    void operator*=(const Vec2& v) { x *= v.x; y *= v.y; }
    Vec2 operator* (const f32 val) const { return { x * val, y * val }; }
    Vec2 operator* (const Vec2& v) const { return { x * v.x, y * v.y }; }
    void operator+=(const f32 val) { x += val; y += val; }
    void operator+=(const Vec2& v) { x += v.x; y += v.y; }
    Vec2 operator+ (const f32 val) const { return { x + val, y + val }; }
    Vec2 operator+ (const Vec2& v) const { return { x + v.x, y + v.y }; }
    void operator-=(const f32 val) { x -= val; y -= val; }
    void operator-=(const Vec2& v) { x -= v.x; y -= v.y; }
    Vec2 operator- (const f32 val) const { return { x - val, y - val }; }
    Vec2 operator- (const Vec2& v) const { return { x - v.x, y - v.y }; }

    void AddScaledVector(const Vec2& vec, f32 scale) {
        x += vec.x * scale;
        y += vec.y * scale;
    }
    void HadamardInPlace(const Vec2& other) {
        x *= other.x;
        y *= other.y;
    }
    void NormaliseInPlace() {
        f32 inv = 1.0f/(x*x + y*y);
        x *= inv;
        y *= inv;
    }

    static Vec2 Up() { return Vec2(0.0, 1.0); }
    static Vec2 Down() { return Vec2(0.0, -1.0); }
    static Vec2 Right() { return Vec2(1.0, 0.0); }
    static Vec2 Left() { return Vec2(-1.0, 0.0); }
};

inline Vec2 operator- (const Vec2& v) { return {-v.x, -v.y}; }

f32 SqrMagnitude(const Vec2& v);
f32 Magnitude(const Vec2& v);
Vec2 Normalised(const Vec2& v);
f32 Dot(const Vec2& u, const Vec2& v);
Vec2 Hadamard(const Vec2& u, const Vec2& v);
Vec2 Lerp(const Vec2& u, const Vec2& v, f32 t);
Vec2 LerpClamped(const Vec2& u, const Vec2& v, f32 t);

struct Vec3 {
    Vec3(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}
    Vec3() {}

    f32 x;
    f32 y;
    f32 z;
    f32 _pad;

    void operator*=(const f32 val) { x *= val; y *= val; z *= val; }
    void operator*=(const Vec3& v) { x *= v.x; y *= v.y; z *= v.z; }
    Vec3 operator* (const f32 val) const { return { x * val, y * val, z * val }; }
    Vec3 operator* (const Vec3& v) const { return { x * v.x, y * v.y, z * v.z }; }
    void operator+=(const f32 val) { x += val; y += val; z += val; }
    void operator+=(const Vec3& v) { x += v.x; y += v.y; z += v.z; }
    Vec3 operator+ (const f32 val) const { return { x + val, y + val, z + val }; }
    Vec3 operator+ (const Vec3& v) const { return { x + v.x, y + v.y, z + v.z }; }
    void operator-=(const f32 val) { x -= val; y -= val; z -= val; }
    void operator-=(const Vec3& v) { x -= v.x; y -= v.y; z -= v.z; }
    Vec3 operator- (const f32 val) const { return { x - val, y - val, z - val }; }
    Vec3 operator- (const Vec3& v) const { return { x - v.x, y - v.y, z - v.z }; }

    void AddScaledVector(const Vec3& vec, f32 scale) {
        x += vec.x * scale;
        y += vec.y * scale;
        z += vec.z * scale;
    }
    
    void HadamardInPlace(const Vec3& other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
    }

    void NormaliseInPlace() {
        f32 inv = 1.0f/(x*x + y*y + z*z);
        x *= inv;
        y *= inv;
        z *= inv;
    }

    // We use Right-Handed y-UP coordinates (i.e. z points out of screen)

    static Vec3 Up() { return Vec3(0.0, 1.0, 0.0); }
    static Vec3 Down() { return Vec3(0.0, -1.0, 0.0); }
    static Vec3 Right() { return Vec3(1.0, 0.0, 0.0); }
    static Vec3 Left() { return Vec3(-1.0, 0.0, 0.0); }
    static Vec3 Forward() { return Vec3(0.0, 0.0, 1.0); }
    static Vec3 Back() { return Vec3(0.0, 0.0, -1.0); }
};

inline Vec3 operator- (const Vec3& v) { return {-v.x, -v.y, -v.z}; }

f32 SqrMagnitude(const Vec3& v);
f32 Magnitude(const Vec3& v);
Vec3 Normalised(const Vec3& v);
f32 Dot(const Vec3& u, const Vec3& v);
Vec3 Hadamard(const Vec3& u, const Vec3& v);
Vec3 Lerp(const Vec3& u, const Vec3& v, f32 t);
Vec3 LerpClamped(const Vec3& u, const Vec3& v, f32 t);
Vec3 Cross(const Vec3& u, const Vec3& v);

// TODO: Vec4 API

struct Vec4 {
    Vec4(f32 x, f32 y, f32 z, f32 w) : x(x), y(y), z(z), w(w) {}
    Vec4() {}

    f32 x;
    f32 y;
    f32 z;
    f32 w;
};

// TODO: Quat API

struct Quat {
    Quat(f32 x, f32 y, f32 z, f32 w) : x(x), y(y), z(z), w(w) {}
    Quat() {}

    f32 x;
    f32 y;
    f32 z;
    f32 w;
};

struct Mat2x2 {
    Mat2x2(f32 xx, f32 xy, f32 yx, f32 yy) {
        m[0] = xx; m[1] = xy;
        m[2] = yx; m[3] = yy;
    }

    Mat2x2() {}

    f32 m[4] = {};

    void operator*=(const f32& val);
    void operator*=(const Mat2x2& other);
    Mat2x2 operator* (const f32 val) const;
    Mat2x2 operator* (const Mat2x2& other) const;
    void operator+=(const f32 val);
    void operator+=(const Mat2x2& other);
    Mat2x2 operator+ (const f32 val) const;
    Mat2x2 operator+ (const Mat2x2& other) const;
    void operator-=(const f32 val);
    void operator-=(const Mat2x2& other);
    Mat2x2 operator- (const f32 val) const;
    Mat2x2 operator- (const Mat2x2& other) const;
};

struct Mat3x3 {
    Mat3x3(f32 xx, f32 xy, f32 xz, f32 yx, f32 yy, f32 yz, f32 zx, f32 zy, f32 zz) {
        m[0] = xx; m[1] = xy; m[2] = xz; 
        m[3] = yx; m[4] = yy; m[5] = yz;
        m[6] = zx; m[5] = zy; m[8] = zz;
    }

    Mat3x3() {}

    f32 m[9] = {};

    void operator*=(const f32& val);
    void operator*=(const Mat3x3& other);
    Mat3x3 operator* (const f32 val) const;
    Mat3x3 operator* (const Mat3x3& other) const;
    void operator+=(const f32 val);
    void operator+=(const Mat3x3& other);
    Mat3x3 operator+ (const f32 val) const;
    Mat3x3 operator+ (const Mat3x3& other) const;
    void operator-=(const f32 val);
    void operator-=(const Mat3x3& other);
    Mat3x3 operator- (const f32 val) const;
    Mat3x3 operator- (const Mat3x3& other) const;
};

struct Mat4x3 {
    Mat4x3() {}

    f32 m[12] = {};
};

struct Mat4x4 {
    Mat4x4(f32 xx, f32 xy, f32 xz, f32 xw, f32 yx, f32 yy, f32 yz, f32 yw, f32 zx, f32 zy, f32 zz, f32 zw, f32 wx, f32 wy, f32 wz, f32 ww) {
        m[0] = xx; m[1] = xy; m[2] = xz; m[3] = xw;
        m[4] = yx; m[5] = yy; m[6] = yz; m[7] = yw;
        m[8] = zx; m[9] = zy; m[10] = zz; m[11] = zw;
        m[12] = wx; m[13] = wy; m[14] = wz; m[15] = ww;
    }

    Mat4x4() {}

    f32 m[16] = {};

    void operator*=(const f32& val);
    void operator*=(const Mat4x4& other);
    Mat4x4 operator* (const f32 val) const;
    Mat4x4 operator* (const Mat4x4& other) const;
    void operator+=(const f32 val);
    void operator+=(const Mat4x4& other);
    Mat4x4 operator+ (const f32 val) const;
    Mat4x4 operator+ (const Mat4x4& other) const;
    void operator-=(const f32 val);
    void operator-=(const Mat4x4& other);
    Mat4x4 operator- (const f32 val) const;
    Mat4x4 operator- (const Mat4x4& other) const;

    static Mat4x4 Identity() {
        Mat4x4 out = {};
        out.m[0] = 1.0f;
        out.m[5] = 1.0f;
        out.m[10] = 1.0f;
        out.m[15] = 1.0f;
        return out;
    }

    void Rotate(Vec3 euler);
    void Rotate(f32 x, f32 y, f32 z);

    static Mat4x4 Translation(Vec3 displacement);
    static Mat4x4 Translation(f32 x, f32 y, f32 z);

    // Order of rotations is Roll -> Yaw -> Pitch (Z -> Y -> X)
    static Mat4x4 Rotation(Vec3 euler);
    static Mat4x4 Rotation(f32 x, f32 y, f32 z);

    static Mat4x4 Scale(Vec3 scale);
    static Mat4x4 Scale(f32 x, f32 y, f32 z);

    static Mat4x4 ViewLookDir(Vec3 position, Vec3 forward, Vec3 up);
    static Mat4x4 ViewLookAt(Vec3 position, Vec3 target, Vec3 up);
    static Mat4x4 Orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);
    static Mat4x4 Perspective(f32 fovyInDeg, f32 aspect, f32 near, f32 far);
};

Vec3 ApplyTransform(const Mat4x4& transform, Vec3 vec);

void DebugPrint(const Vec2& v);
void DebugPrint(const Vec3& v);
void DebugPrint(const Vec4& v);

void DebugPrint(const Mat2x2& v);
void DebugPrint(const Mat3x3& v);
void DebugPrint(const Mat4x4& v);