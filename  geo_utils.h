
#ifndef LINEAR_TRANSFORM_H
#define LINEAR_TRANSFORM_H

#include <cmath>

#define ftype double

struct point2d {
    ftype x, y;
    point2d() : x(0), y(0) {}
    point2d(ftype x, ftype y): x(x), y(y) {}
    point2d& operator+=(const point2d &t) {
        x += t.x;
        y += t.y;
        return *this;
    }
    point2d& operator-=(const point2d &t) {
        x -= t.x;
        y -= t.y;
        return *this;
    }
    point2d& operator*=(ftype t) {
        x *= t;
        y *= t;
        return *this;
    }
    point2d& operator/=(ftype t) {
        x /= t;
        y /= t;
        return *this;
    }
    point2d operator+(const point2d &t) const {
        return point2d(*this) += t;
    }
    point2d operator-(const point2d &t) const {
        return point2d(*this) -= t;
    }
    point2d operator*(ftype t) const {
        return point2d(*this) *= t;
    }
    point2d operator/(ftype t) const {
        return point2d(*this) /= t;
    }
};

point2d operator*(ftype a, const point2d& b) {
    return b * a;
}

struct point3d {
    ftype x, y, z;
    point3d() : x(0), y(0), z(0) {}
    point3d(ftype x, ftype y, ftype z): x(x), y(y), z(z) {}
    point3d& operator+=(const point3d &t) {
        x += t.x;
        y += t.y;
        z += t.z;
        return *this;
    }
    point3d& operator-=(const point3d &t) {
        x -= t.x;
        y -= t.y;
        z -= t.z;
        return *this;
    }
    point3d& operator*=(ftype t) {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }
    point3d& operator/=(ftype t) {
        x /= t;
        y /= t;
        z /= t;
        return *this;
    }
    point3d operator+(const point3d &t) const {
        return point3d(*this) += t;
    }
    point3d operator-(const point3d &t) const {
        return point3d(*this) -= t;
    }
    point3d operator*(ftype t) const {
        return point3d(*this) *= t;
    }
    point3d operator/(ftype t) const {
        return point3d(*this) /= t;
    }
};

point3d operator*(ftype a, const point3d& b) {
    return b * a;
}

ftype dot(const point2d& a, const point2d& b) {
    return a.x * b.x + a.y * b.y;
}

ftype dot(const point3d& a, const point3d& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

ftype norm(const point2d& a) {
    return dot(a, a);
}

double abs_val(const point2d& a) {
    return sqrt(norm(a));
}

double proj(const point2d& a, const point2d& b) {
    return dot(a, b) / abs_val(b);
}

double angle(const point2d& a, const point2d& b) {
    return acos(dot(a, b) / abs_val(a) / abs_val(b));
}

point3d cross(const point3d& a, const point3d& b) {
    return point3d(a.y * b.z - a.z * b.y,
                  a.z * b.x - a.x * b.z,
                  a.x * b.y - a.y * b.x);
}

ftype triple(const point3d& a, const point3d& b, const point3d& c) {
    return dot(a, cross(b, c));
}

ftype cross(const point2d& a, const point2d& b) {
    return a.x * b.y - a.y * b.x;
}

point2d intersect(const point2d& a1, const point2d& d1, const point2d& a2, const point2d& d2) {
    return a1 + (cross(a2 - a1, d2) / cross(d1, d2)) * d1;
}

point3d intersect(const point3d& a1, const point3d& n1,
                 const point3d& a2, const point3d& n2,
                 const point3d& a3, const point3d& n3) {
    point3d x(n1.x, n2.x, n3.x);
    point3d y(n1.y, n2.y, n3.y);
    point3d z(n1.z, n2.z, n3.z); 
    point3d d(dot(a1, n1), dot(a2, n2), dot(a3, n3));
    return point3d(triple(d, y, z),
                  triple(x, d, z),
                  triple(x, y, d)) / triple(n1, n2, n3);
}

#undef ftype

#endif // LINEAR_TRANSFORM_H
