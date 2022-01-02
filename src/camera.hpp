#pragma once
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "common.hpp"

class camera
{
public:
    camera(
            point3 lookfrom,
            point3 lookat,
            vec3 vup,
            double vfov, // vertical field-of-view in degrees
            double aspect_ratio,
            double aperture,
            double focus_dist)
    {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        this->w = (lookfrom - lookat).unit_vector();
        this->u = vup.cross(w).unit_vector();
        this->v = w.cross(u);

        this->origin = lookfrom;
        this->horizontal = focus_dist * viewport_width * u;
        this->vertical = focus_dist * viewport_height * v;
        this->lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;

        this->lens_radius = aperture / 2;
    }

    ray get_ray(double s, double t)
    {
        vec3 rd = lens_radius * vec3::random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();

        return {origin + offset,
                lower_left_corner + s * horizontal + t * vertical - origin - offset};
    }

private:
    vec3 u, v, w;
    point3 origin;
    vec3 horizontal;
    vec3 vertical;
    point3 lower_left_corner;

    double lens_radius;
};

#endif