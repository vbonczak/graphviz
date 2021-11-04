#pragma once

#include "cgp/cgp.hpp" // Give access to the complete CGP library
#include <iostream> 
#include "scenebase.h"

void droite_souris(scene_base& scene, const vec2& cursor, vec3& pos, vec3& direction);

bool intersection_droite(vec3& intersect, const vec3& droite_position, const vec3& droite_direction, const vec3& center, float radius);

bool intersection_plan(vec3& intersect, const vec3& droite_position, const vec3& droite_direction, const vec3& normal, const vec3& sphere_position);

vec2 glfw_cursor_coordinates_window(GLFWwindow* window);

bool glfw_mouse_pressed_left(GLFWwindow* window);

bool glfw_key_shift_pressed(GLFWwindow* window);
