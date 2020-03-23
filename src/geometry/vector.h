//
// Created by peter on 2020-03-21.
//

#ifndef TINYRENDERER_VECTOR_H
#define TINYRENDERER_VECTOR_H

#include <cmath>
#include <cassert>
#include <iostream>
#include <vector>


namespace Geometry {
    template<class T>
    struct Vec2 {
        union {
            struct {
                T u, v;
            };
            struct {
                T x, y;
            };
            T raw[2];
        };

        Vec2() : u(0), v(0) {}

        Vec2(T _u, T _v) : u(_u), v(_v) {}

        inline Vec2<T> operator+(const Vec2<T> &V) const { return Vec2<T>(u + V.u, v + V.v); }

        inline Vec2<T> operator-(const Vec2<T> &V) const { return Vec2<T>(u - V.u, v - V.v); }

        inline T operator*(const Vec2<T> &V) const { return u * V.u + v * V.u; }

        inline T &operator[](int i) {
            assert(i >= 0 && i < 2);
            if (i == 0) return u;
            else return v;
        }

        inline const T &operator[](int i) const {
            assert(i >= 0 && i < 2);
            if (i == 0) return u;
            else return v;
        }

        inline Vec2<T> &operator+=(const Vec2<T> &V) {
            u += V.u;
            v += V.v;
            return *this;
        }

        template<class>
        friend std::ostream &operator<<(std::ostream &s, Vec2<T> &v);
    };

    template<typename T, typename U>
    inline Vec2<T> operator*(const U &lhs, const Vec2<T> &rhs) {
        return Vec2<T>(rhs.u * lhs, rhs.v * lhs);
    }

    template<typename T, typename U>
    inline Vec2<T> operator*(const Vec2<T> &lhs, const U &rhs) {
        return Vec2<T>(lhs.u * rhs, lhs.v * rhs);
    }

    template<typename T, typename U>
    inline Vec2<T> operator/(const Vec2<T> &lhs, const U &rhs) {
        return Vec2<T>(lhs.u / rhs, lhs.v / rhs);
    }

    template<class T>
    struct Vec3 {
        union {
            struct {
                T x, y, z;
            };
            struct {
                T ivert, iuv, inorm;
            };
            T raw[3];
        };

        Vec3() : x(0), y(0), z(0) {}

        Vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

        inline Vec3<T> operator^(const Vec3<T> &v) const {
            return Vec3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
        }

        inline Vec3<T> operator+(const Vec3<T> &v) const { return Vec3<T>(x + v.x, y + v.y, z + v.z); }

        inline Vec3<T> operator-(const Vec3<T> &v) const { return Vec3<T>(x - v.x, y - v.y, z - v.z); }

        inline T operator*(const Vec3<T> &v) const { return x * v.x + y * v.y + z * v.z; }

        inline T &operator[](int i) {
            assert(i >= 0 && i < 3);
            if (i == 0) return x;
            else if (i == 1) return y;
            else return z;
        }

        inline const T &operator[](int i) const {
            assert(i >= 0 && i < 3);
            if (i == 0) return x;
            else if (i == 1) return y;
            else return z;
        }

        inline Vec3<T> &operator+=(const Vec3<T> &V) {
            x += V.x;
            y += V.y;
            z += V.z;
            return *this;
        }

        float norm() const { return std::sqrt(x * x + y * y + z * z); }

        Vec3<T> &normalize(T l = 1) {
            *this = (*this) * (l / norm());
            return *this;
        }

        template<class>
        friend std::ostream &operator<<(std::ostream &s, Vec3<T> &v);
    };

    template<typename T, typename U>
    inline Vec3<T> operator*(const U &lhs, const Vec3<T> &rhs) {
        return Vec3<T>(rhs.x * lhs, rhs.y * lhs, rhs.z * lhs);
    }

    template<typename T, typename U>
    inline Vec3<T> operator*(const Vec3<T> &lhs, const U &rhs) {
        return Vec3<T>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
    }

    template<typename T, typename U>
    inline Vec3<T> operator/(const Vec3<T> &lhs, const U &rhs) {
        return Vec3<T>(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
    }


    typedef Vec2<float> Vec2f;
    typedef Vec2<int> Vec2i;
    typedef Vec3<float> Vec3f;
    typedef Vec3<int> Vec3i;

    template<class T>
    std::ostream &operator<<(std::ostream &s, Vec2<T> &v) {
        s << "(" << v.x << ", " << v.y << ")\n";
        return s;
    }

    template<class T>
    std::ostream &operator<<(std::ostream &s, Vec3<T> &v) {
        s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
        return s;
    }
}

#endif //TINYRENDERER_VECTOR_H
