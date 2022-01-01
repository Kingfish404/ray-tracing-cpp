#pragma once
#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "../common.hpp"

struct hit_record;

class material
{
public:
    virtual bool scatter(
            const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered
    ) = 0;
};

#endif