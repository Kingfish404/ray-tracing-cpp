#include "common.hpp"
#include "camera.hpp"
#include "utils/color.hpp"
#include "utils/hittable.hpp"
#include "utils/sphere.hpp"
#include "utils/material.hpp"

#include <iostream>
#include <chrono>
#include <fstream>
#include <thread>
#include <vector>

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
    vec3 unit_direction = r.direction().unit_vector();
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

hittable_list random_scene()
{
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9)
            {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8)
                {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95)
                {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else
                {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

hittable_list single_scene()
{
    hittable_list world;
    // Color
    auto color_yellow = color(0.8, 0.8, 0.0);
    auto color_pink = color(0.7, 0.3, 0.3);
    auto color_blue = color(0.1, 0.2, 0.5);
    auto color_gray = color(0.8, 0.8, 0.8);
    auto color_gloden = color(0.8, 0.6, 0.2);
    auto color_66ccff = color(0.4, 0.8, 1);

    auto material_ground = make_shared<lambertian>(color_yellow);
    auto material_center = make_shared<lambertian>(color_blue);
    auto material_left = make_shared<dielectric>(1.5);
    auto material_right = make_shared<metal>(color_gloden, 1.0);

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.499, material_left));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    return world;
}

int main(int argc, char *argv[])
{
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 720;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    // World
    hittable_list world = (argc > 1 && argv[1][0] == 's') ? single_scene() : random_scene();

    // Camera
    point3 lookfrom{0, 1, 10};
    point3 lookat{0, 0, 0};
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;
    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    // Output and profile setting
    auto num_thr = 1;
    auto num_remaining = image_height;
    auto ***out = new unsigned int **[image_height];
    for (int i = 0; i < image_width; i++)
    {
        out[i] = new unsigned int *[image_width];
        for (int j = 0; j < image_width; j++)
        {
            out[i][j] = new unsigned int[3];
        }
    }

    auto start = std::chrono::system_clock::now();

    // Multi thread render
    std::vector<std::shared_ptr<std::thread>> threads;
    for (int t = 0; t < num_thr; t++)
    {
        auto thr = make_shared<std::thread>(
            [&, t]()
            {
                int start_height = t * (image_height / num_thr);
                int end_height = start_height + (image_height / num_thr);
                for (int i = start_height; i < end_height && i < image_height; i++)
                {
                    std::cout << "\rScanlines remaining: "
                              << ((double)num_remaining--) / image_height
                              << "% " << std::flush;
                    for (int j = 0; j < image_width; ++j)
                    {
                        color pixel_color(0, 0, 0);
                        for (int s = 0; s < samples_per_pixel; ++s)
                        {
                            auto u = (j + random_double()) / (image_width - 1);
                            auto v = (i + random_double()) / (image_height - 1);
                            ray r = cam.get_ray(u, v);
                            pixel_color += ray_color(r, world, max_depth);
                        }
                        write_color(out[i][j], pixel_color, samples_per_pixel);
                    }
                }
            });
        threads.push_back(thr);
    }

    for (auto thr : threads)
    {
        thr->join();
    }

    // Output to file
    std::ofstream file;
    file.open("image.ppm", std::ios::out);
    file << "P3\n"
         << image_width << " " << image_height << "\n255\n";
    for (int i = image_height - 1; i >= 0; i--)
    {
        for (int j = 0; j < image_width; j++)
        {
            file << out[i][j][0] << " "
                 << out[i][j][1] << " "
                 << out[i][j][2] << "\n";
            delete[] out[i][j];
        }
    }
    file.close();

    auto end = std::chrono::system_clock::now();
    std::cout << "\nDone. time cost: " << ((std::chrono::duration<double>)(end - start)).count() << "s\n";
    return 0;
}