#pragma once

#include "cgp/cgp.hpp"

#include "simulation/simulation.hpp"

using namespace std;

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
	cgp::mesh_drawable sphere;
	cgp::mesh_drawable plane;
	cgp::skybox_drawable skybox;


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

	//vector<GLuint> balls_textures;             // Storage of the texture ID used for each ball
};
