#pragma once

#include "cgp/cgp.hpp"

extern float alpha, beta, mu, vepsilon;

struct particle_structure
{
    cgp::vec3 p; // Position
    cgp::vec3 v; // Speed

    cgp::vec3 c; // Color
    float r;     // Radius
    float m;     // mass
};

void simulate(std::vector<particle_structure>& particles, float dt);

