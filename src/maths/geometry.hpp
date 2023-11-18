#pragma once

#include "core/base.h"
#include "maths/linalg.hpp"

struct Point2D {
    Point2D(f32 x, f32 y) : x(x), y(y) {}
    Point2D() {}
    f32 x;
    f32 y;
};

struct Line2D {
    Line2D(const Point2D& origin, const Vec2& direction) : origin(origin), direction(direction) {}
    Line2D(f32 x, f32 y, f32 dx, f32 dy) : origin(x, y), direction(dx, dy) {}
    Line2D() {}
    Point2D origin;
    Vec2 direction;
};

struct Rect2D {
    Rect2D(f32 x, f32 y, f32 w, f32 h) : x(x), y(y), w(w), h(h) {}
    Rect2D() {}
    Rect2D(const Point2D& p1, const Point2D& p2);
    f32 x;
    f32 y;
    f32 w;
    f32 h;
};

struct Point3D {
    Point3D(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}
    Point3D() {}
    f32 x;
    f32 y;
    f32 z;
};

struct Line3D {
    Line3D(const Point3D& origin, const Vec3& direction) : origin(origin), direction(direction) {}
    Line3D(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz) : origin(x, y, z), direction(dx, dy, dz) {}
    Line3D() {}
    Point3D origin;
    Vec3 direction;
};

struct Volume3D {
    Volume3D(f32 x, f32 y, f32 z, f32 w, f32 h, f32 d) : x(x), y(y), z(z), w(w), h(h), d(d) {}
    Volume3D() {}
    Volume3D(const Point3D& p1, const Point3D& p2);
    f32 x;
    f32 y;
    f32 z;
    f32 w;
    f32 h;
    f32 d;
};

struct Plane3D {
    Plane3D(const Point3D& origin, const Vec3& normal) : origin(origin), normal(normal) {}
    Plane3D(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz) : origin(x, y, z), normal(dx, dy, dz) {}
    Plane3D() {}
    Point3D origin;
    Vec3 normal;
};