#pragma once
#include "cgp/cgp.hpp" // Give access to the complete CGP library
#include <iostream> 

using namespace cgp;

struct scene_base {
	void initialize();  // Standard initialization to be called before the animation loop
	void add_balls();
	void display();     // The frame display to be called within the animation loop
	void display_gui(); // The display of the GUI, also called within the animation loop
 
	void refresh_control_positions();
	void queue_init();
	void queue_reinit();


	void mouse_move_callback(GLFWwindow* window, double xpos, double ypos, inputs_interaction_parameters& inputs);
	void mouse_click_callback(GLFWwindow* window, int button, int action, int /*mods*/);
	scene_environment_basic environment;
};