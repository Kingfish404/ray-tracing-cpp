/**
 * @file common.hpp
 * @author Jin Yu (lambda.jinyu@gmail.com)
 *
 */

#pragma once
#ifndef COMMON_HPP
#define COMMON_HPP

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>
#include <random>

// Using

using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180.0;
}

// @brief Returns a random real in [0,1).
inline double random_double()
{
    return rand() / (RAND_MAX + 1.0);
}

// @brief Returns a random real in [min,max]
inline double random_double(double min, double max)
{
    return min + (max - min) * random_double();
}

inline double clamp(double x, double min, double max)
{
    if (x < min)
    {
        return min;
    }
    if (x > max)
    {
        return max;
    }
    return x;
}

// Common Headers

#include "utils/vec3.hpp"
#include "utils/ray.hpp"
#include "utils/hittable.hpp"
#include "utils/material.hpp"

#endif