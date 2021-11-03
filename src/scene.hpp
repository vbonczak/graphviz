#pragma once

#include "cgp/cgp.hpp"

#include "simulation/simulation.hpp"

using namespace std;
using namespace cgp;

// The element of the GUI that are not already stored in other structures
struct gui_parameters {
	bool display_frame = false;
	bool display_wireframe = false;
};



// The structure of the custom scene
struct scene_structure {

	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	cgp::mesh_drawable global_frame;          // The standard global frame
	gui_parameters gui;                       // Standard GUI element storage

											  // Standard environment controler with spherical coordinates
	cgp::scene_environment_basic environment;


	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //
	mesh_drawable sphere;
	mesh_drawable plane;
	skybox_drawable skybox;

	int cur_control;
	
	vec3 ctrl1, ctrl2;
	float control_radius;
	mesh_drawable control_sphere1;

	float queue_radius;
	mesh_drawable queue;
	mesh_drawable control_sphere2;


	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //
	cgp::timer_event_periodic timer;
	std::vector<particle_structure> particles;
	// ****************************** //
	// Functions
	// ****************************** //

	void initialize();  // Standard initialization to be called before the animation loop
	void display();     // The frame display to be called within the animation loop
	void display_gui(); // The display of the GUI, also called within the animation loop

	void add_balls();
	//void simulation_step(float dt);
	void sphere_display();
	void refresh_control_positions();
	//vector<GLuint> balls_textures;             // Storage of the texture ID used for each ball
};
