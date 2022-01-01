#pragma once
#ifndef VEC3_HPP
#define VEC3_HPP

#include "../common.hpp"

#include <cmath>
#include <iostream>

using std::sqrt;

class vec3
{
public:
    vec3() : e{0, 0, 0}
    {};

    vec3(double e0, double e1, double e2) : e{e0, e1, e2}
    {
    }

    double x() const
    {
        return e[0];
    }

    double y() const
    {
        return e[1];
    }

    double z() const
    {
        return e[2];
    }

    inline static vec3 random()
    {
        return {random_double(), random_double(), random_double()};
    }

    inline static vec3 random(double min, double max)
    {
        return {random_double(min, max), random_double(min, max), random_double(min, max)};
    }

    inline static vec3 random_in_unit_sphere()
    {
        while (true)
        {
            auto p = vec3::random(-1, 1);
            if (p.length_squared() >= 1)
            {
                continue;
            }
            return p;
        }
    }

    inline static vec3 random_unit_vector()
    {
        return unit_vector(random_in_unit_sphere());
    }

    inline static vec3 random_in_hemisphere(const vec3 &normal)
    {
        vec3 in_unit_sphere = random_in_unit_sphere();
        if (in_unit_sphere.dot(normal) > 0)
        {
            return in_unit_sphere;
        }
        return -in_unit_sphere;
    }

    // Return true if the vector is close to zero in all dimensions.
    bool near_zero() const
    {
        const auto s = 1e-8;
        return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
    }

    vec3 operator-() const
    {
        return {-e[0], -e[1], -e[2]};
    }

    double operator[](int i) const
    {
        return e[i];
    }

    double &operator[](int i)
    {
        return e[i];
    }

    vec3 operator+=(const vec3 &v)
    {
        for (int i = 0; i < 3; ++i)
        {
            e[i] += v[i];
        }
        return *this;
    }

    vec3 &operator*=(const double t)
    {
        for (double &i: e)
        {
            i *= t;
        }
        return *this;
    }

    vec3 &operator/=(const double t)
    {
        return *this *= 1 / t;
    }

    double length() const
    {
        return sqrt(length_squared());
    }

    double length_squared() const
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    inline double dot(const vec3 &v) const
    {
        auto &u = *this;
        return (u[0] * v[0] + u[1] * v[1] + u[2] * v[2]);
    }

    inline vec3 cross(const vec3 &v) const
    {
        auto &u = *this;
        return vec3(u[1] * v[2] - u[2] * v[1],
                    u[2] * v[0] - u[0] * v[2],
                    u[0] * v[1] - u[1] * v[0]);
    }

    /**
     * @brief Get unit length: [-1.0, 1.0]{3}
     *
     * @return vec3
     */
    inline static vec3 unit_vector(vec3);

private:
    double e[3];
};

// Type aliases for vec3

using point3 = vec3; // 3D point
using color = vec3;  // RGB color

// vec3 Utility Functions

inline std::ostream &operator<<(std::ostream &out, const vec3 &v)
{
    return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v)
{
    return {u[0] + v[0], u[1] + v[1], u[2] + v[2]};
}

inline vec3 operator-(const vec3 &u, const vec3 &v)
{
    return {u[0] - v[0], u[1] - v[1], u[2] - v[2]};
}

inline vec3 operator*(const vec3 &u, const vec3 &v)
{
    return {u[0] * v[0], u[1] * v[1], u[2] * v[2]};
}

inline vec3 operator*(double t, const vec3 &v)
{
    return {t * v[0], t * v[1], t * v[2]};
}

inline vec3 operator*(const vec3 &v, double t)
{
    return t * v;
}

inline vec3 operator/(vec3 v, double t)
{
    return (1 / t) * v;
}

vec3 vec3::unit_vector(vec3 v)
{
    return v / v.length();
};

vec3 reflect(const vec3 &v, const vec3 &n)
{
    return v + 2 * (-v.dot(n)) * n;
}

#endif