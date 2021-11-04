#pragma once

#include "cgp/cgp.hpp"

#include "simulation3D/simulation3D.hpp"
#include "simulation/simulation.hpp"
#include "scenebase.h"
#include "utils.h"

using namespace std;
using namespace cgp;

// The element of the GUI that are not already stored in other structures
struct gui_parameters3D {
	bool display_frame = false;
	bool display_wireframe = false;
};



// The structure of the custom scene
struct scene_structure3D : scene_base {

	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	cgp::mesh_drawable global_frame;          // The standard global frame
	gui_parameters3D gui;                       // Standard GUI element storage

											  // Standard environment controler with spherical coordinates
	cgp::scene_environment_basic environment;


	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //
	mesh_drawable sphere;
	mesh_drawable plane;
	skybox_drawable skybox;
	cgp::segments_drawable cube_wireframe;

	int cur_control;

	vec3 ctrl_pos;
	float control_radius;
	mesh_drawable control_sphere;



	float queue_radius;
	float theta;
	float cue_white_dist;
	float queue_length;
	bool queue_waiting;
	mesh_drawable queue;



	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //
	cgp::timer_event_periodic timer;

	/// <summary>
	/// La première boule est la blanche.
	/// </summary>
	std::vector<boule_structure> boules;

	// ****************************** //
	// Functions
	// ****************************** //

	void initialize();  // Standard initialization to be called before the animation loop
	void init_objects();
	void display();     // The frame display to be called within the animation loop
	void display_gui(); // The display of the GUI, also called within the animation loop

	void add_balls();
	//void simulation_step(float dt);
	void sphere_display();
	void refresh_control_positions();
	void queue_init();
	void queue_reinit();
	void launch_ball();


	void mouse_move_callback(GLFWwindow* window, double xpos, double ypos, inputs_interaction_parameters& inputs);
	void mouse_click_callback(GLFWwindow* window, int button, int action, int /*mods*/);
	//vector<GLuint> balls_textures;             // Storage of the texture ID used for each ball
};
