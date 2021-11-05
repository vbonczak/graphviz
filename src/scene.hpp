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

	//-1 if no currrent control point selected, index of the point otherwise (allows to have more if necessary)
	int cur_control;
	//The current 3D position of the control sphere
	vec3 ctrl_pos;
	//Radius of the control sphere
	float control_radius;
	//The control sphere
	mesh_drawable control_sphere;

	//Rayon de la queue
	float queue_radius;
	//Angle
	float theta;
	//Distance à la boule blanche
	float cue_white_dist;
	//Longueur de la queue
	float queue_length;
	//permet de faire disparaître la queue lors du tir
	bool queue_waiting;
	//La queue 
	mesh_drawable queue; 

	GLint texture_bois;
	//Bords
	mesh_drawable bord1;
	mesh_drawable bord2;
	mesh_drawable bord3;
	mesh_drawable bord4;

	//Trous
	mesh_drawable hole1;
	mesh_drawable hole2;
	mesh_drawable hole3;
	mesh_drawable hole4;
	mesh_drawable hole5;
	mesh_drawable hole6;

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
	void init_objects();//All other initialisation processes
	void display();     // The frame display to be called within the animation loop
	void display_gui(); // The display of the GUI, also called within the animation loop

	void add_balls();//Ajoute les boules
	void sphere_display();
	void refresh_control_positions();
	void queue_init();
	void queue_reinit();
	void shoot_ball();
	//vector<GLuint> balls_textures;             // Storage of the texture ID used for each ball (future use)
};
