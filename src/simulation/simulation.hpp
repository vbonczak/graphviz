#pragma once

#include "cgp/cgp.hpp"

extern float alpha, beta, beta_feutre, mu, vepsilon;

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
    //GLint text; //texture

    bool in_play; //Si la boule est toujours en jeu
};

void simulate(std::vector<boule_structure>& boules, float dt);

bool over_hole(boule_structure& boule);

void simulate3d(std::vector<boule_structure>& boules, float dt);

bool over_hole3d(boule_structure& boule);