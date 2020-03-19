//
// Created by peter on 2020-02-20.
//

#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <cmath>
#include <cassert>
#include <iostream>
#include <vector>


template<class t>
struct Vec2 {
    union {
        struct {
            t u, v;
        };
        struct {
            t x, y;
        };
        t raw[2];
    };

    Vec2() : u(0), v(0) {}

    Vec2(t _u, t _v) : u(_u), v(_v) {}

    inline Vec2<t> operator+(const Vec2<t> &V) const { return Vec2<t>(u + V.u, v + V.v); }

    inline Vec2<t> operator-(const Vec2<t> &V) const { return Vec2<t>(u - V.u, v - V.v); }

    // TODO: Fix ordering so f * V and V * f works
    inline Vec2<t> operator*(float f) const { return Vec2<t>(u * f, v * f); }

    inline Vec2<t> operator/(float f) const { return Vec2<t>(u / f, v / f); }

    inline t &operator[](int i) {
        assert(i >= 0 && i < 2);
        if (i == 0) return u;
        else return v;
    }

    inline const t &operator[](int i) const {
        assert(i >= 0 && i < 2);
        if (i == 0) return u;
        else return v;
    }

    inline Vec2<t>& operator+=(const Vec2<t> &V) {
        u += V.u;
        v += V.v;
        return *this;
    }

    template<class>
    friend std::ostream &operator<<(std::ostream &s, Vec2<t> &v);
};

template<class t>
struct Vec3 {
    union {
        struct {
            t x, y, z;
        };
        struct {
            t ivert, iuv, inorm;
        };
        t raw[3];
    };

    Vec3() : x(0), y(0), z(0) {}

    Vec3(t _x, t _y, t _z) : x(_x), y(_y), z(_z) {}

    inline Vec3<t> operator^(const Vec3<t> &v) const {
        return Vec3<t>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }

    inline Vec3<t> operator+(const Vec3<t> &v) const { return Vec3<t>(x + v.x, y + v.y, z + v.z); }

    inline Vec3<t> operator-(const Vec3<t> &v) const { return Vec3<t>(x - v.x, y - v.y, z - v.z); }

    inline Vec3<t> operator*(float f) const { return Vec3<t>(x * f, y * f, z * f); }

    inline Vec3<t> operator/(float f) const { return Vec3<t>(x / f, y / f, z / f); }

    inline t operator*(const Vec3<t> &v) const { return x * v.x + y * v.y + z * v.z; }

    inline t &operator[](int i) {
        assert(i >= 0 && i < 3);
        if (i == 0) return x;
        else if (i == 1) return y;
        else return z;
    }

    inline const t &operator[](int i) const {
        assert(i >= 0 && i < 3);
        if (i == 0) return x;
        else if (i == 1) return y;
        else return z;
    }

    inline Vec3<t>& operator+=(const Vec3<t> &V) {
        x += V.x;
        y += V.y;
        z += V.z;
        return *this;
    }

    float norm() const { return std::sqrt(x * x + y * y + z * z); }

    Vec3<t> &normalize(t l = 1) {
        *this = (*this) * (l / norm());
        return *this;
    }

    template<class>
    friend std::ostream &operator<<(std::ostream &s, Vec3<t> &v);
};

typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;

template<class t>
std::ostream &operator<<(std::ostream &s, Vec2<t> &v) {
    s << "(" << v.x << ", " << v.y << ")\n";
    return s;
}

template<class t>
std::ostream &operator<<(std::ostream &s, Vec3<t> &v) {
    s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
    return s;
}

const int DEFAULT_M_SIZE = 4;

class Matrix {
    std::vector<std::vector<float>> m;
    int rows, cols;
public:
    explicit Matrix(int r = DEFAULT_M_SIZE, int c = DEFAULT_M_SIZE);

    inline int nrows();

    inline int ncols();

    static Matrix identity(int dimensions);

    Matrix transpose();

    Matrix inverse();

    const std::vector<float> &operator[](int i) const;

    std::vector<float> &operator[](int i);

    Matrix operator*(const Matrix &mat) const;

    Matrix operator*(const Matrix &mat);

    friend std::ostream &operator<<(std::ostream &s, Matrix &m);
};

#endif //__GEOMETRY_H__
