#pragma once
#ifndef COLOR_HPP
#define COLOR_HPP

#include "vec3.hpp"
#include "../common.hpp"

#include <iostream>

/**
 * @brief Write color [0, 1.0]{3} to [0, 255]{3}
 * @param pixel_out int[3] array
 * @param pixel_color color: [0, 1.0]{3}
 */
void write_color(unsigned int out[3], color pixel_color, int samples_per_pixel)
{
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by the number of samples.
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    // Write the translated [0,255] value of each color component.
    out[0] = static_cast<int>(256 * clamp(r, 0.0, 0.999));
    out[1] = static_cast<int>(256 * clamp(g, 0.0, 0.999));
    out[2] = static_cast<int>(256 * clamp(b, 0.0, 0.999));
}

#endif