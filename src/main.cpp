#include "common.hpp"
#include "camera.hpp"
#include "utils/color.hpp"
#include "utils/hittable_list.hpp"
#include "utils/sphere.hpp"
#include "utils/material.hpp"

#include <iostream>
#include <chrono>

color ray_color(const ray &r, const hittable &world, int depth)
{
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
    {
        return {0, 0, 0};
    }

    hit_record rec;
    if (world.hit(r, 0.001, infinity, rec))
    {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            return attenuation * ray_color(scattered, world, depth - 1);
        }
        return {0, 0, 0};
    }
    vec3 unit_direction = vec3::unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main()
{
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 720;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    // Color
    auto color_yellow = color(0.8, 0.8, 0.0);
    auto color_pink = color(0.7, 0.3, 0.3);
    auto color_gray = color(0.8, 0.8, 0.8);
    auto color_gloden = color(0.8, 0.6, 0.2);
    auto color_66ccff = color(0.4, 0.8, 1);

    // World
    hittable_list world;
    auto material_ground = make_shared<lambertian>(color_66ccff);
    auto material_center = make_shared<lambertian>(color_pink);
    auto material_left = make_shared<metal>(color_gray, 0.3);
    auto material_right = make_shared<metal>(color_gloden, 1.0);

    world.add(make_shared<sphere>(point3(0, -100.5, -1.0), 100, material_ground));
    world.add(make_shared<sphere>(point3(0, 0, -1.0), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0, 0, -1.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3(1.0, 0, -1.0), 0.5, material_right));

    // Camera
    camera cam;

    // Render
    std::cout << "P3\n"
              << image_width << " " << image_height << "\n255\n";


    auto start = std::chrono::system_clock::now();

    for (int j = image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i)
        {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s)
            {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cerr << "cost:" << diff.count() << "s\n";
    return 0;
}