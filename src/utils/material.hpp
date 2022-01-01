#pragma once
#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "../common.hpp"

class material
{
public:
    virtual bool scatter(
        const ray &r_in, const hit_record &rec,
        color &attenuation, ray &scattered) const = 0;
};

class lambertian : public material
{
public:
    lambertian(const color &a) : albedo(a){};

    bool scatter(
        const ray &r_in, const hit_record &rec,
        color &attenuation, ray &scattered)
        const override
    {
        auto scatter_direction = rec.normal + vec3::random_unit_vector();

        // Catch degenerate scatter direction
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

private:
    color albedo;
};

class metal : public material
{
public:
    metal(const color &a, double f) : albedo(a), fuzz(f < 1 ? f : 1){};

    bool scatter(
        const ray &r_in, const hit_record &rec,
        color &attenuation, ray &scattered) const override
    {
        vec3 reflected = reflect(vec3::unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * vec3::random_in_unit_sphere());
        attenuation = albedo;
        return (scattered.direction().dot(rec.normal) > 0);
    };

private:
    color albedo;
    double fuzz;
};

#endif