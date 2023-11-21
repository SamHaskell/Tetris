#include "maths/geometry.hpp"

Rect2D::Rect2D(const Point2D& p1, const Point2D& p2) {
    if (p1.x < p2.x) {
        x = p1.x;
        w = p2.x - p1.x;
    } else {
        x = p2.x;
        w = p1.x - p2.x;
    }

    if (p1.y > p2.y) {
        y = p1.y;
        h = p2.y - p1.y;
    } else {
        y = p2.y;
        h = p1.y - p2.y;
    }
}

Volume3D::Volume3D(const Point3D& p1, const Point3D& p2) {
    if (p1.x < p2.x) {
        x = p1.x;
        w = p2.x - p1.x;
    } else {
        x = p2.x;
        w = p1.x - p2.x;
    }

    if (p1.y < p2.y) {
        y = p1.y;
        h = p2.y - p1.y;
    } else {
        y = p2.y;
        h = p1.y - p2.y;
    }

    if (p1.z < p2.z) {
        z = p1.z;
        d = p2.z - p1.z;
    } else {
        z = p2.z;
        d = p1.z - p2.z;
    }
}