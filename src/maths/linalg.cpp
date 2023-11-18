#include "maths/numerics.hpp"
#include "maths/linalg.hpp"

f32 SqrMagnitude(const Vec2& v) {
    return Dot(v, v);
}

f32 Magnitude(const Vec2& v) {
    return sqrt(Dot(v, v));
}

Vec2 Normalised(const Vec2& v) {
    f32 inv = 1.0f/SqrMagnitude(v);
    return v * inv;
}

f32 Dot(const Vec2& u, const Vec2& v) {
    return u.x*v.x + u.y*v.y;
}

Vec2 Hadamard(const Vec2& u, const Vec2& v) {
    return {u.x*v.x, u.y*v.y};
}

Vec2 Lerp(const Vec2& u, const Vec2& v, f32 t) {
    return { Lerp(u.x, v.x, t), Lerp(u.y, v.y, t) };
}

Vec2 LerpClamped(const Vec2& u, const Vec2& v, f32 t) {
    f32 clamped = Clamp(t, 0.0f, 1.0f);
    return { Lerp(u.x, v.x, clamped), Lerp(u.y, v.y, clamped) };
}

f32 SqrMagnitude(const Vec3& v) {
    return Dot(v, v);
}

f32 Magnitude(const Vec3& v) {
    return sqrt(Dot(v, v));
}

Vec3 Normalised(const Vec3& v) {
    f32 inv = 1.0f/SqrMagnitude(v);
    return v * inv;
}

f32 Dot(const Vec3& u, const Vec3& v) {
    return u.x*v.x + u.y*v.y + u.z*v.z;
}

Vec3 Hadamard(const Vec3& u, const Vec3& v) {
    return {u.x*v.x, u.y*v.y, u.z*v.z};
}

Vec3 Lerp(const Vec3& u, const Vec3& v, f32 t) {
    return { Lerp(u.x, v.x, t), Lerp(u.y, v.y, t), Lerp(u.z, v.z, t) };
}

Vec3 LerpClamped(const Vec3& u, const Vec3& v, f32 t) {
    f32 clamped = Clamp(t, 0.0f, 1.0f);
    return { Lerp(u.x, v.x, clamped), Lerp(u.y, v.y, clamped), Lerp(u.z, v.z, clamped) };
}

Vec3 Cross(const Vec3& u, const Vec3& v) {
    return {
        u.y*v.z - u.z*v.y,
        u.z*v.x - u.x*v.z,
        u.x*v.y - u.y*v.x
    };
}

// Mat2x2 Operator Overloads

void Mat2x2::operator*=(const f32& val) {
    m[0] *= val;
    m[1] *= val;
    m[2] *= val;
    m[3] *= val;
}

void Mat2x2::operator*=(const Mat2x2& other) {
    (*this) = (*this) * other;
}

Mat2x2 Mat2x2::operator* (const f32 val) const {
    return Mat2x2(
        m[0] * val,
        m[1] * val,
        m[2] * val,
        m[3] * val
    );
}

Mat2x2 Mat2x2::operator* (const Mat2x2& other) const {
    return Mat2x2(
        m[0] * other.m[0] + m[2] * other.m[1],
        m[1] * other.m[0] + m[3] * other.m[1],
        m[0] * other.m[2] + m[2] * other.m[3],
        m[1] * other.m[2] + m[3] * other.m[3]  
    );
}

void Mat2x2::operator+=(const f32 val) {
    m[0] += val;
    m[1] += val;
    m[2] += val;
    m[3] += val;
}

void Mat2x2::operator+=(const Mat2x2& other) {
    m[0] += other.m[0];
    m[1] += other.m[1];
    m[2] += other.m[2];
    m[3] += other.m[3]; 
}

Mat2x2 Mat2x2::operator+ (const f32 val) const {
    return Mat2x2(
        m[0] + val,
        m[1] + val,
        m[2] + val,
        m[3] + val
    );
}

Mat2x2 Mat2x2::operator+ (const Mat2x2& other) const {
    return Mat2x2(
        m[0] + other.m[0],
        m[1] + other.m[1],
        m[2] + other.m[2],
        m[3] + other.m[3]
    );
}

void Mat2x2::operator-=(const f32 val) {
    m[0] -= val;
    m[1] -= val;
    m[2] -= val;
    m[3] -= val;
}

void Mat2x2::operator-=(const Mat2x2& other) {
    m[0] -= other.m[0];
    m[1] -= other.m[1];
    m[2] -= other.m[2];
    m[3] -= other.m[3]; 
}

Mat2x2 Mat2x2::operator- (const f32 val) const {
    return Mat2x2(
        m[0] - val,
        m[1] - val,
        m[2] - val,
        m[3] - val
    );
}

Mat2x2 Mat2x2::operator- (const Mat2x2& other) const {
    return Mat2x2(
        m[0] - other.m[0],
        m[1] - other.m[1],
        m[2] - other.m[2],
        m[3] - other.m[3]
    );
}

// Mat3x3 Operator Overloads

void Mat3x3::operator*=(const f32& val) {
    m[0] *= val; m[1] *= val; m[2] *= val;
    m[3] *= val; m[4] *= val; m[5] *= val; 
    m[6] *= val; m[7] *= val; m[8] *= val;
}

void Mat3x3::operator*=(const Mat3x3& other) {
    (*this) = (*this) * other;
}

Mat3x3 Mat3x3::operator* (const f32 val) const {
    return Mat3x3(
        m[0] * val, m[1] * val, m[2] * val, 
        m[3] * val, m[4] * val, m[5] * val, 
        m[6] * val, m[7] * val, m[8] * val
    );
}

Mat3x3 Mat3x3::operator* (const Mat3x3& other) const {
    return Mat3x3(
        m[0] * other.m[0] + m[3] * other.m[1] + m[6] * other.m[2],
        m[1] * other.m[0] + m[4] * other.m[1] + m[7] * other.m[2],
        m[2] * other.m[0] + m[5] * other.m[1] + m[8] * other.m[2],

        m[0] * other.m[3] + m[3] * other.m[4] + m[6] * other.m[5],
        m[1] * other.m[3] + m[4] * other.m[4] + m[7] * other.m[5],
        m[2] * other.m[3] + m[5] * other.m[4] + m[8] * other.m[5],

        m[0] * other.m[6] + m[3] * other.m[7] + m[6] * other.m[8],
        m[1] * other.m[6] + m[4] * other.m[7] + m[7] * other.m[8],
        m[2] * other.m[6] + m[5] * other.m[7] + m[8] * other.m[8]
    );
}

void Mat3x3::operator+=(const f32 val) {
    m[0] += val; m[1] += val; m[2] += val;
    m[3] += val; m[4] += val; m[5] += val; 
    m[6] += val; m[7] += val; m[8] += val;
}

void Mat3x3::operator+=(const Mat3x3& other) {
    m[0] += other.m[0]; m[1] += other.m[1]; m[2] += other.m[2];
    m[3] += other.m[3]; m[4] += other.m[4]; m[5] += other.m[5]; 
    m[6] += other.m[6]; m[7] += other.m[7]; m[8] += other.m[8];
}

Mat3x3 Mat3x3::operator+ (const f32 val) const {
    return Mat3x3(
        m[0] + val, m[1] + val, m[2] + val,
        m[3] + val, m[4] + val, m[5] + val,
        m[6] + val, m[7] + val, m[8] + val
    );
}

Mat3x3 Mat3x3::operator+ (const Mat3x3& other) const {
    return Mat3x3(
        m[0] + other.m[0], m[1] + other.m[1], m[2] + other.m[2],
        m[3] + other.m[3], m[4] + other.m[4], m[5] + other.m[5],
        m[6] + other.m[6], m[7] + other.m[7], m[8] + other.m[8]
    );
}

void Mat3x3::operator-=(const f32 val) {
    m[0] -= val; m[1] -= val; m[2] -= val;
    m[3] -= val; m[4] -= val; m[5] -= val; 
    m[6] -= val; m[7] -= val; m[8] -= val;
}

void Mat3x3::operator-=(const Mat3x3& other) {
    m[0] -= other.m[0]; m[1] -= other.m[1]; m[2] -= other.m[2];
    m[3] -= other.m[3]; m[4] -= other.m[4]; m[5] -= other.m[5]; 
    m[6] -= other.m[6]; m[7] -= other.m[7]; m[8] -= other.m[8];
}

Mat3x3 Mat3x3::operator- (const f32 val) const {
    return Mat3x3(
        m[0] - val, m[1] - val, m[2] - val,
        m[3] - val, m[4] - val, m[5] - val,
        m[6] - val, m[7] - val, m[8] - val
    );
}

Mat3x3 Mat3x3::operator- (const Mat3x3& other) const {
    return Mat3x3(
        m[0] - other.m[0], m[1] - other.m[1], m[2] - other.m[2],
        m[3] - other.m[3], m[4] - other.m[4], m[5] - other.m[5],
        m[6] - other.m[6], m[7] - other.m[7], m[8] - other.m[8]
    );
}

// Mat4x3 Operator Overloads



// Mat4x4 Operator Overloads

void Mat4x4::operator*=(const f32& val) {
    m[0] *= val; m[1] *= val; m[2] *= val; m[3] *= val;
    m[4] *= val; m[5] *= val; m[6] *= val; m[7] *= val;
    m[8] *= val; m[9] *= val; m[10] *= val; m[11] *= val;
    m[12] *= val; m[13] *= val; m[14] *= val; m[15] *= val;
}

void Mat4x4::operator*=(const Mat4x4& other) {
    (*this) = (*this) * other;
}

Mat4x4 Mat4x4::operator* (const f32 val) const {
    return Mat4x4(
        m[0] * val, m[1] * val, m[2] * val, m[3] * val,
        m[4] * val, m[5] * val, m[6] * val, m[7] * val,
        m[8] * val, m[9] * val, m[10] * val, m[11] * val,
        m[12] * val, m[13] * val, m[14] * val, m[15] * val
    );
}

Mat4x4 Mat4x4::operator* (const Mat4x4& other) const {
    return Mat4x4(
        m[0] * other.m[0] + m[4] * other.m[1] + m[8] * other.m[2] + m[12] * other.m[3],
        m[1] * other.m[0] + m[5] * other.m[1] + m[9] * other.m[2] + m[13] * other.m[3],
        m[2] * other.m[0] + m[6] * other.m[1] + m[10] * other.m[2] + m[14] * other.m[3],
        m[3] * other.m[0] + m[7] * other.m[1] + m[11] * other.m[2] + m[15] * other.m[3],

        m[0] * other.m[4] + m[4] * other.m[5] + m[8] * other.m[6] + m[12] * other.m[7],
        m[1] * other.m[4] + m[5] * other.m[5] + m[9] * other.m[6] + m[13] * other.m[7],
        m[2] * other.m[4] + m[6] * other.m[5] + m[10] * other.m[6] + m[14] * other.m[7],
        m[3] * other.m[4] + m[7] * other.m[5] + m[11] * other.m[6] + m[15] * other.m[7],

        m[0] * other.m[8] + m[4] * other.m[9] + m[8] * other.m[10] + m[12] * other.m[11],
        m[1] * other.m[8] + m[5] * other.m[9] + m[9] * other.m[10] + m[13] * other.m[11],
        m[2] * other.m[8] + m[6] * other.m[9] + m[10] * other.m[10] + m[14] * other.m[11],
        m[3] * other.m[8] + m[7] * other.m[9] + m[11] * other.m[10] + m[15] * other.m[11],

        m[0] * other.m[12] + m[4] * other.m[13] + m[8] * other.m[14] + m[12] * other.m[15],
        m[1] * other.m[12] + m[5] * other.m[13] + m[9] * other.m[14] + m[13] * other.m[15],
        m[2] * other.m[12] + m[6] * other.m[13] + m[10] * other.m[14] + m[14] * other.m[15],
        m[3] * other.m[12] + m[7] * other.m[13] + m[11] * other.m[14] + m[15] * other.m[15]
    );
}

void Mat4x4::operator+=(const f32 val) {
    m[0] += val; m[1] += val; m[2] += val; m[3] += val;
    m[4] += val; m[5] += val; m[6] += val; m[7] += val;
    m[8] += val; m[9] += val; m[10] += val; m[11] += val;
    m[12] += val; m[13] += val; m[14] += val; m[15] += val;
}

void Mat4x4::operator+=(const Mat4x4& other) {
    m[0] += other.m[0]; m[1] += other.m[1]; m[2] += other.m[2]; m[3] += other.m[3];
    m[4] += other.m[4]; m[5] += other.m[5]; m[6] += other.m[6]; m[7] += other.m[7];
    m[8] += other.m[8]; m[9] += other.m[9]; m[10] += other.m[10]; m[11] += other.m[11];
    m[12] += other.m[12]; m[13] += other.m[13]; m[14] += other.m[14]; m[15] += other.m[15];
}

Mat4x4 Mat4x4::operator+ (const f32 val) const {
    return Mat4x4(
        m[0] + val, m[1] + val, m[2] + val, m[3] + val,
        m[4] + val, m[5] + val, m[6] + val, m[7] + val,
        m[8] + val, m[9] + val, m[10] + val, m[11] + val,
        m[12] + val, m[13] + val, m[14] + val, m[15] + val
    );
}

Mat4x4 Mat4x4::operator+ (const Mat4x4& other) const {
    return Mat4x4(
        m[0] + other.m[0], m[1] + other.m[1], m[2] + other.m[2], m[3] + other.m[3],
        m[4] + other.m[4], m[5] + other.m[5], m[6] + other.m[6], m[7] + other.m[7],
        m[8] + other.m[8], m[9] + other.m[9], m[10] + other.m[10], m[11] + other.m[11],
        m[12] + other.m[12], m[13] + other.m[13], m[14] + other.m[14], m[15] + other.m[15]
    );
}

void Mat4x4::operator-=(const f32 val) {
    m[0] -= val; m[1] -= val; m[2] -= val; m[3] -= val;
    m[4] -= val; m[5] -= val; m[6] -= val; m[7] -= val;
    m[8] -= val; m[9] -= val; m[10] -= val; m[11] -= val;
    m[12] -= val; m[13] -= val; m[14] -= val; m[15] -= val;
}

void Mat4x4::operator-=(const Mat4x4& other) {
    m[0] -= other.m[0]; m[1] -= other.m[1]; m[2] -= other.m[2]; m[3] -= other.m[3];
    m[4] -= other.m[4]; m[5] -= other.m[5]; m[6] -= other.m[6]; m[7] -= other.m[7];
    m[8] -= other.m[8]; m[9] -= other.m[9]; m[10] -= other.m[10]; m[11] -= other.m[11];
    m[12] -= other.m[12]; m[13] -= other.m[13]; m[14] -= other.m[14]; m[15] -= other.m[15];
}

Mat4x4 Mat4x4::operator- (const f32 val) const {
    return Mat4x4(
        m[0] - val, m[1] - val, m[2] - val, m[3] - val,
        m[4] - val, m[5] - val, m[6] - val, m[7] - val,
        m[8] - val, m[9] - val, m[10] - val, m[11] - val,
        m[12] - val, m[13] - val, m[14] - val, m[15] - val
    );
}

Mat4x4 Mat4x4::operator- (const Mat4x4& other) const {
    return Mat4x4(
        m[0] - other.m[0], m[1] - other.m[1], m[2] - other.m[2], m[3] - other.m[3],
        m[4] - other.m[4], m[5] - other.m[5], m[6] - other.m[6], m[7] - other.m[7],
        m[8] - other.m[8], m[9] - other.m[9], m[10] - other.m[10], m[11] - other.m[11],
        m[12] - other.m[12], m[13] - other.m[13], m[14] - other.m[14], m[15] - other.m[15]
    );
}

void Mat4x4::Rotate(Vec3 euler) {
    (*this) = Mat4x4::Rotation(euler) * (*this);
}

void Mat4x4::Rotate(f32 x, f32 y, f32 z) {
    (*this) = Mat4x4::Rotation(x, y, z) * (*this);
}

Mat4x4 Mat4x4::Translation(Vec3 displacement) {
    return Mat4x4::Translation(displacement.x, displacement.y, displacement.z);
}

Mat4x4 Mat4x4::Translation(f32 x, f32 y, f32 z) {
    Mat4x4 out = Mat4x4::Identity();
    out.m[12] =  x;
    out.m[13] =  y;
    out.m[14] = z;
    return out;
}

Mat4x4 Mat4x4::Rotation(Vec3 euler) {
    return Mat4x4::Rotation(euler.x, euler.y, euler.z);
}

/*
    NOTE: Can definitely be optimised, look at more performant trig calcs.
*/

Mat4x4 Mat4x4::Rotation(f32 x, f32 y, f32 z) {
    Mat4x4 out = Mat4x4::Identity();

    f32 sinX = sinf(DEG2RAD * x);
    f32 cosX = cosf(DEG2RAD * x);
    f32 sinY = sinf(DEG2RAD * y);
    f32 cosY = cosf(DEG2RAD * y);
    f32 sinZ = sinf(DEG2RAD * z);
    f32 cosZ = cosf(DEG2RAD * z);

    out.m[0]  = + (cosY * cosZ);
    out.m[1]  = - (cosY * sinZ);
    out.m[2]  = + (sinY);
    
    out.m[4]  = + (sinX * sinY * cosZ) + (cosX * sinZ);
    out.m[5]  = - (sinX * sinY * sinZ) + (cosX * cosZ); 
    out.m[6]  = - (sinX * cosY);

    out.m[8]  = - (cosX * sinY * cosZ) + (sinX * sinZ);
    out.m[9]  = + (cosX * sinY * sinZ) + (sinX * cosZ);
    out.m[10] = + (cosX * cosY);

    return out;
}

Mat4x4 Mat4x4::Scale(Vec3 scale) {
    return Mat4x4::Scale(scale.x, scale.y, scale.z);
}

Mat4x4 Mat4x4::Scale(f32 x, f32 y, f32 z) {
    Mat4x4 out = Mat4x4::Identity();
    out.m[0] = x;
    out.m[5] = y;
    out.m[10] = z;
    return out;
}

/*
    NOTE: For view matrices we need the transformation which brings the camera back 
    to the origin and then rotates it such that it points along the negative z-axis.
    i.e. translation of negative the camera pos, followed by rotation towards {0.0, 0.0, -1.0};
*/

Mat4x4 Mat4x4::ViewLookDir(Vec3 position, Vec3 forward, Vec3 up) {
    Vec3 test = -position;
    Mat4x4 out = Mat4x4::Translation(test);
    forward = - Normalised(forward);
    up.NormaliseInPlace();
    Vec3 left = Cross(up, forward);

    Mat4x4 invRot = Mat4x4::Identity();
    invRot.m[0] = left.x;
    invRot.m[4] = left.y;
    invRot.m[8] = left.z;

    invRot.m[1] = up.x;
    invRot.m[5] = up.y;
    invRot.m[9] = up.z;

    invRot.m[2] = forward.x;
    invRot.m[6] = forward.y;
    invRot.m[10] = forward.z;

    return out * invRot;
}

Mat4x4 Mat4x4::ViewLookAt(Vec3 position, Vec3 target, Vec3 up) {
    return Mat4x4::ViewLookDir(position, target - position, up);
}

Mat4x4 Mat4x4::Orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far) {
    Mat4x4 out = Mat4x4::Identity();
    f32 invWidth = 1.0f / (right - left);
    f32 invHeight = 1.0f / (top - bottom);
    f32 invDepth = 1.0f / (far - near);
    out.m[0] = 2.0f * invWidth;
    out.m[5] = 2.0f * invHeight;
    out.m[10] = -2.0f * invDepth;
    out.m[12] = - (right + left) * invWidth;
    out.m[13] = - (top + bottom) * invHeight;
    out.m[14] = - (far + near) * invDepth;
    out.m[15] = 1.0f;
    return out;
}

Mat4x4 Mat4x4::Perspective(f32 fovyInDeg, f32 aspect, f32 near, f32 far) {
    f32 invTan = 1.0f/tanf(DEG2RAD * fovyInDeg / 2.0f);
    f32 invZinterval = 1.0f / (far - near);

    Mat4x4 out = Mat4x4::Identity();
    out.m[0] = (1.0f / aspect) * invTan;
    out.m[5] = invTan;
    out.m[10] = - (far + near) * invZinterval;
    out.m[14] = - 2.0f * far * near * invZinterval;
    out.m[11] = -1.0f;
    out.m[15] = 0.0f;
    return out;
}

// Other

Vec3 ApplyTransform(const Mat4x4& transform, Vec3 vec) {
    return Vec3(
        transform.m[0] * vec.x + transform.m[4] * vec.y + transform.m[8] * vec.z + transform.m[12] * 1.0f,
        transform.m[1] * vec.x + transform.m[5] * vec.y + transform.m[9] * vec.z + transform.m[13] * 1.0f,
        transform.m[2] * vec.x + transform.m[6] * vec.y + transform.m[10] * vec.z + transform.m[14] * 1.0f                
    );
}

// Printing and Debugging

void DebugPrint(const Vec2& v) {
    printf("( %8.4f, %8.4f )\n", v.x, v.y);
    printf("\n");
}

void DebugPrint(const Vec3& v) {
    printf("( %8.4f, %8.4f, %8.4f )\n", v.x, v.y, v.z);
    printf("\n");
}

void DebugPrint(const Vec4& v) {
    printf("( %8.4f, %8.4f, %8.4f, %8.4f )\n", v.x, v.y, v.z, v.w);
    printf("\n");
}

void DebugPrint(const Mat2x2& v) {
    printf("| %8.4f, %8.4f |\n", v.m[0], v.m[2]);
    printf("| %8.4f, %8.4f |\n", v.m[1], v.m[3]);
    printf("\n");
}

void DebugPrint(const Mat3x3& v) {
    printf("| %8.4f, %8.4f, %8.4f |\n", v.m[0], v.m[3], v.m[6]);
    printf("| %8.4f, %8.4f, %8.4f |\n", v.m[1], v.m[4], v.m[7]);
    printf("| %8.4f, %8.4f, %8.4f |\n", v.m[2], v.m[5], v.m[8]);
    printf("\n");
}

void DebugPrint(const Mat4x4& v) {
    printf("| %8.4f, %8.4f, %8.4f, %8.4f |\n", v.m[0], v.m[4], v.m[8], v.m[12]);
    printf("| %8.4f, %8.4f, %8.4f, %8.4f |\n", v.m[1], v.m[5], v.m[9], v.m[13]);
    printf("| %8.4f, %8.4f, %8.4f, %8.4f |\n", v.m[2], v.m[6], v.m[10], v.m[14]);
    printf("| %8.4f, %8.4f, %8.4f, %8.4f |\n", v.m[3], v.m[7], v.m[11], v.m[15]);
}