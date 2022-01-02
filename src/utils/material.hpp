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
    explicit lambertian(const color &a) : albedo(a){};

    bool scatter(
        const ray &r_in, const hit_record &rec,
        color &attenuation, ray &scattered) const override
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
        color &attenuation, ray &scattered) const override;
    ;

private:
    color albedo;
    double fuzz;
};

bool metal::scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const
{
    vec3 reflected = reflect(r_in.direction().unit_vector(), rec.normal);
    scattered = ray(rec.p, reflected + fuzz * vec3::random_in_unit_sphere());
    attenuation = albedo;
    return (scattered.direction().dot(rec.normal) > 0);
}

class dielectric : public material
{
public:
    explicit dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    bool scatter(
        const ray &r_in, const hit_record &rec,
        color &attenuation, ray &scattered) const override
    {
        attenuation = color(1.0, 1.0, 1.0);
        double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

        vec3 unit_direction = r_in.direction().unit_vector();
        double cos_theta = fmin((-unit_direction).dot(rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        vec3 direction;

        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
        {
            direction = reflect(unit_direction, rec.normal);
        }
        else
        {
            direction = refract(unit_direction, rec.normal, refraction_ratio);
        }

        scattered = ray(rec.p, direction);
        return true;
    }

private:
    double ir; // Index of Refraction

    static double reflectance(double cosine, double ref_idx)
    {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};

#endif