/**
 * @file common.hpp
 * @author Jin Yu (jin-yu@acm.org)
 * @brief Named rtweekend.h in source of raytracing class
 * 
 */

#pragma once
#ifndef COMMON_HPP
#define COMMON_HPP

#include <cmath>
#include <limits>
#include <memory>

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

// Common Headers

#include "utils/ray.hpp"
#include "utils/vec3.hpp"

#endif