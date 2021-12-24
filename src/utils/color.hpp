#pragma once
#ifndef COLOR_HPP
#define COLOR_HPP

#include "vec3.hpp"

#include <iostream>

/**
 * @brief Write color [0, 1.0]{3} to [0, 255]{3}
 * 
 * @param out ostream
 * @param pixel_color color: [0, 1.0]{3}
 */
void write_color(std::ostream &out, color pixel_color)
{
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

#endif