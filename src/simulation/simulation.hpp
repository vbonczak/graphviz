#pragma once

#include "cgp/cgp.hpp"

extern float alpha, beta, mu, vepsilon;

//Dimensions du plateau
extern float H;
extern float L;
extern float h;

struct boule_structure
{
    cgp::vec3 p; // Position
    cgp::vec3 v; // Speed

    cgp::vec3 c; // Color
    float r;     // Radius
    float m;     // mass
    GLint text; //texture
};

void simulate(std::vector<boule_structure>& boules, float dt);

